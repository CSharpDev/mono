<%@ Page Language="c#" AutoEventWireup="false" Codebehind="DataList_SelectedIndex.aspx.cs" Inherits="GHTTests.System_Web_dll.System_Web_UI_WebControls.DataList_SelectedIndex" %>
<%@ Register TagPrefix="cc1" Namespace="GHTWebControls" Assembly="MainsoftWebApp" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
	<HEAD>
		<title>DataList_SelectedIndex</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="Visual Basic .NET 7.1" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<script LANGUAGE="JavaScript">
        function ScriptTest()
        {
            var theform;
		    if (window.navigator.appName.toLowerCase().indexOf("netscape") > -1) {
			    theform = document.forms["Form1"];
		    }
		    else {
			    theform = document.Form1;
		    }
        }
		</script>
	</HEAD>
	<body>
		<FORM id="Form1" method="post" runat="server">
			<P>
				<cc1:GHTSubTest id="GHTSubTest1" runat="server" Width="232px" Height="112px" Description="Not set">
					<asp:DataList id="DataList1" runat="server" Height="112px" Width="136px">
						<ItemTemplate>
							Regular item.
						</ItemTemplate>
						<SelectedItemTemplate>
							Selected Item!
						</SelectedItemTemplate>
					</asp:DataList>
				</cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest2" runat="server" Width="232px" Height="112px" Description="Set to legal value">
					<asp:DataList id="DataList2" runat="server" Height="120px" Width="144px" SelectedIndex="2">
						<ItemTemplate>
							Regular item.
						</ItemTemplate>
						<selectedItemTemplate>
							Selected Item!
						</selectedItemTemplate>
					</asp:DataList>
				</cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest3" runat="server" Width="232px" Height="112px" Description="Higher then the number of items.">
					<asp:DataList id="DataList3" runat="server" Height="122px" Width="136px" SelectedIndex="10">
						<ItemTemplate>
							Regular item.
						</ItemTemplate>
						<selectedItemTemplate>
							Selected Item!
						</selectedItemTemplate>
					</asp:DataList>
				</cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest4" runat="server" Width="232px" Height="112px" Description="Illegal value - less then -1">
					<asp:DataList id="DataList4" runat="server" Height="130px" Width="168px">
						<ItemTemplate>
							Regular item.
						</ItemTemplate>
						<selectedItemTemplate>
							Selected Item!
						</selectedItemTemplate>
					</asp:DataList>
				</cc1:GHTSubTest></P>
			<P>&nbsp;</P>
		</FORM>
	</body>
</HTML>
