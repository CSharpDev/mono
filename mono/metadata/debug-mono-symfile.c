#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/rawbuffer.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/debug-mono-symfile.h>

#include <fcntl.h>
#include <unistd.h>


static int
load_symfile (MonoSymbolFile *symfile)
{
	MonoSymbolFileMethodEntry *me, *me_end;
	const char *ptr, *start, *end;
	guint64 magic;
	long version;

	ptr = start = symfile->raw_contents;

	magic = *((guint64 *) ptr)++;
	if (magic != MONO_SYMBOL_FILE_MAGIC) {
		g_warning ("Symbol file %s has is not a mono symbol file", symfile->file_name);
		return FALSE;
	}

	version = *((guint32 *) ptr)++;
	if (version != MONO_SYMBOL_FILE_VERSION) {
		g_warning ("Symbol file %s has incorrect line number table version "
			   "(expected %d, got %ld)", symfile->file_name,
			   MONO_SYMBOL_FILE_VERSION, version);
		return FALSE;
	}

	symfile->offset_table = (MonoSymbolFileOffsetTable *) ptr;

	/*
	 * Read method table.
	 *
	 */

	symfile->method_table = g_hash_table_new_full (g_direct_hash, g_direct_equal,
						       NULL, (GDestroyNotify) g_free);

	ptr = symfile->raw_contents + symfile->offset_table->method_table_offset;
	end = ptr + symfile->offset_table->method_table_size;

	me = (MonoSymbolFileMethodEntry *) ptr;
	me_end = ((MonoSymbolFileMethodEntry *) end);

	for (; me < me_end; me++) {
		MonoMethod *method = mono_get_method (symfile->image, me->token, NULL);

		if (!method)
			continue;

		g_hash_table_insert (symfile->method_table, method, me);
	}

	return TRUE;
}

MonoSymbolFile *
mono_debug_open_mono_symbol_file (MonoImage *image, const char *filename, gboolean emit_warnings)
{
	MonoSymbolFile *symfile;
	off_t file_size;
	void *ptr;
	int fd;

	fd = open (filename, O_RDWR);
	if (fd == -1) {
		if (emit_warnings)
			g_warning ("Can't open symbol file: %s", filename);
		return NULL;
	}

	file_size = lseek (fd, 0, SEEK_END);
	lseek (fd, 0, SEEK_SET);

	if (file_size == (off_t) -1) {
		if (emit_warnings)
			g_warning ("Can't get size of symbol file: %s", filename);
		return NULL;
	}

	ptr = mono_raw_buffer_load (fd, 1, 0, file_size);
	if (!ptr) {
		if (emit_warnings)
			g_warning ("Can't read symbol file: %s", filename);
		return NULL;
	}

	symfile = g_new0 (MonoSymbolFile, 1);
	symfile->fd = fd;
	symfile->file_name = g_strdup (filename);
	symfile->image = image;
	symfile->raw_contents = ptr;
	symfile->raw_contents_size = file_size;

	if (!load_symfile (symfile)) {
		mono_debug_close_mono_symbol_file (symfile);
		return NULL;
	}

	return symfile;
}

void
mono_debug_close_mono_symbol_file (MonoSymbolFile *symfile)
{
	if (!symfile)
		return;

	if (symfile->raw_contents)
		mono_raw_buffer_free (symfile->raw_contents);
	if (symfile->fd)
		close (symfile->fd);

	g_free (symfile->file_name);
	g_free (symfile);
}

gchar *
mono_debug_find_source_location (MonoSymbolFile *symfile, MonoMethod *method, guint32 offset,
				 guint32 *line_number)
{
	MonoSymbolFileMethodEntry *me;
	MonoSymbolFileLineNumberEntry *lne, *lne_end;
	const char *ptr, *end, *source_file;

	if (!symfile->method_table)
		return NULL;

	me = g_hash_table_lookup (symfile->method_table, method);
	if (!me)
		return NULL;

	source_file = symfile->raw_contents + me->source_file_offset;

	ptr = symfile->raw_contents + me->line_number_table_offset;
	end = symfile->raw_contents + symfile->offset_table->line_number_table_offset +
		symfile->offset_table->line_number_table_size;

	lne = (MonoSymbolFileLineNumberEntry *) ptr;
	lne_end = ((MonoSymbolFileLineNumberEntry *) end);

	for (; (lne < lne_end) && lne->row; lne++) {
		g_message (G_STRLOC ": %d - %d", lne->row, lne->offset);

		if (lne->offset < offset)
			continue;

		if (line_number) {
			*line_number = lne->row;
			return g_strdup (source_file);
		} else
			return g_strdup_printf ("%s:%d", source_file, lne->row);
	}

	return NULL;
}
