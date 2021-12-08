VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form Form2 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "文档类型选择"
   ClientHeight    =   2370
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   2700
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2370
   ScaleWidth      =   2700
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "关闭"
      Height          =   375
      Left            =   1440
      TabIndex        =   2
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton Command1 
      Caption         =   "选择"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   1800
      Width           =   1095
   End
   Begin ComctlLib.ListView ListView1 
      Height          =   1695
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   2655
      _ExtentX        =   4683
      _ExtentY        =   2990
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      _Version        =   327682
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   2
      BeginProperty ColumnHeader(1) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "文档类型"
         Object.Width           =   1482
      EndProperty
      BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   1
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "文档描述"
         Object.Width           =   1482
      EndProperty
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Any) As Long
Private Const LVM_SETEXTENDEDLISTVIEWSTYLE As Long = &H1000& + 54
Private Const LVS_EX_FULLROWSELECT As Long = &H20&           'applies to report mode only


Private Sub Command1_Click()
    If Not ListView1.SelectedItem Is Nothing Then
        Form1.txtArchiveType = ListView1.SelectedItem.SubItems(1)
    End If
    Unload Me
End Sub

Private Sub Command2_Click()
    Unload Me
End Sub

Private Sub Form_Load()
    ListView1.ListItems.Add , , "RAREXE"
    ListView1.ListItems(ListView1.ListItems.Count).SubItems(1) = "1"
 
    ListView1.ListItems.Add , , "ZIPEXE"
    ListView1.ListItems(ListView1.ListItems.Count).SubItems(1) = "2"
 
    ListView1.ListItems.Add , , "NSISEXE"
    ListView1.ListItems(ListView1.ListItems.Count).SubItems(1) = "3"

    ListView1.ListItems.Add , , "CABEXE"
    ListView1.ListItems(ListView1.ListItems.Count).SubItems(1) = "4"

 
    SendMessage ListView1.hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT
 
End Sub



Private Sub ListView1_DblClick()
    If Not ListView1.SelectedItem Is Nothing Then
        Form1.txtArchiveType = ListView1.SelectedItem.SubItems(1)
    End If
    Unload Me
End Sub
