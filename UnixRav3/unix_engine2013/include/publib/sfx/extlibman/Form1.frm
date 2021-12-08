VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "ExtLib(SFX)管理器 Ver1.00"
   ClientHeight    =   6975
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9645
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   ScaleHeight     =   6975
   ScaleWidth      =   9645
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtSearchDesc 
      Height          =   285
      Left            =   480
      TabIndex        =   49
      Text            =   "txtSearchDesc"
      Top             =   4200
      Width           =   4095
   End
   Begin VB.CommandButton cmdSearchModify 
      Caption         =   "修改"
      Height          =   255
      Left            =   1680
      TabIndex        =   48
      Top             =   3720
      Width           =   615
   End
   Begin VB.CommandButton cmdTypeSelect 
      Caption         =   "选择"
      Height          =   255
      Left            =   7560
      TabIndex        =   47
      Top             =   2400
      Width           =   735
   End
   Begin ComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   46
      Top             =   6720
      Width           =   9645
      _ExtentX        =   17013
      _ExtentY        =   450
      SimpleText      =   ""
      _Version        =   327682
      BeginProperty Panels {0713E89E-850A-101B-AFC0-4210102A8DA7} 
         NumPanels       =   3
         BeginProperty Panel1 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Object.Width           =   3422
            MinWidth        =   3422
            Text            =   "文件版本"
            TextSave        =   "文件版本"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel2 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Object.Width           =   5009
            MinWidth        =   5009
            Text            =   "文件日期"
            TextSave        =   "文件日期"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel3 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   1
            Object.Width           =   8484
            MinWidth        =   4304
            Text            =   "CRC"
            TextSave        =   "CRC"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
      EndProperty
   End
   Begin ComctlLib.ListView lvwSearch 
      Height          =   3135
      Left            =   120
      TabIndex        =   41
      Top             =   480
      Width           =   4455
      _ExtentX        =   7858
      _ExtentY        =   5530
      View            =   3
      LabelEdit       =   1
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
         Text            =   "ID"
         Object.Width           =   706
      EndProperty
      BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   1
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "描述"
         Object.Width           =   1764
      EndProperty
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   8640
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdInfoModify 
      Caption         =   "修改"
      Height          =   255
      Left            =   6000
      TabIndex        =   40
      Top             =   2040
      Width           =   615
   End
   Begin VB.CommandButton cmdInfoDel 
      Caption         =   "删除"
      Height          =   255
      Left            =   5400
      TabIndex        =   39
      Top             =   2040
      Width           =   615
   End
   Begin VB.CommandButton cmdInfoAdd 
      Caption         =   "增加"
      Height          =   255
      Left            =   4800
      TabIndex        =   38
      Top             =   2040
      Width           =   615
   End
   Begin VB.TextBox txtIString 
      Height          =   285
      Left            =   7080
      TabIndex        =   37
      Text            =   "txtIString"
      Top             =   3480
      Width           =   2415
   End
   Begin VB.TextBox txtIPos 
      Height          =   285
      Left            =   5640
      TabIndex        =   36
      Text            =   "txtIPos"
      Top             =   3480
      Width           =   735
   End
   Begin VB.TextBox txtSFXPos 
      Height          =   285
      Left            =   8640
      TabIndex        =   33
      Text            =   "txtSFXPos"
      Top             =   3120
      Width           =   855
   End
   Begin VB.TextBox txtArchiveString 
      Height          =   285
      Left            =   6000
      TabIndex        =   31
      Text            =   "txtArchiveString"
      Top             =   3120
      Width           =   1575
   End
   Begin VB.CommandButton cmdPEStringModify 
      Caption         =   "修改"
      Height          =   255
      Left            =   8160
      TabIndex        =   29
      Top             =   6480
      Width           =   615
   End
   Begin VB.CommandButton cmdPEStringDel 
      Caption         =   "删除"
      Height          =   255
      Left            =   7440
      TabIndex        =   28
      Top             =   6480
      Width           =   615
   End
   Begin VB.CommandButton cmdPEStringAdd 
      Caption         =   "增加"
      Height          =   255
      Left            =   6720
      TabIndex        =   27
      Top             =   6480
      Width           =   615
   End
   Begin VB.TextBox txtPEString 
      Height          =   1005
      Left            =   6720
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   26
      Text            =   "Form1.frx":0000
      Top             =   5400
      Width           =   2655
   End
   Begin VB.TextBox txtArchiveType 
      Height          =   285
      Left            =   8400
      TabIndex        =   25
      Text            =   "txtArchiveType"
      Top             =   2400
      Width           =   1095
   End
   Begin VB.CommandButton cmdVersionModify 
      Caption         =   "修改"
      Height          =   255
      Left            =   8040
      TabIndex        =   23
      Top             =   4320
      Width           =   615
   End
   Begin VB.CommandButton cmdVersionDel 
      Caption         =   "删除"
      Height          =   255
      Left            =   7320
      TabIndex        =   22
      Top             =   4320
      Width           =   615
   End
   Begin VB.CommandButton cmdVersionAdd 
      Caption         =   "增加"
      Height          =   255
      Left            =   6600
      TabIndex        =   21
      Top             =   4320
      Width           =   615
   End
   Begin VB.TextBox txtWeb 
      Height          =   285
      Left            =   7320
      TabIndex        =   20
      Text            =   "txtWeb"
      Top             =   2760
      Width           =   2175
   End
   Begin VB.TextBox txtCorp 
      Height          =   285
      Left            =   5280
      TabIndex        =   19
      Text            =   "txtCorp"
      Top             =   2760
      Width           =   1455
   End
   Begin VB.TextBox txtVersionDesc 
      Height          =   285
      Left            =   8280
      TabIndex        =   18
      Text            =   "txtVersionDesc"
      Top             =   3960
      Width           =   1095
   End
   Begin VB.TextBox txtVersion 
      Height          =   285
      Left            =   7080
      TabIndex        =   17
      Text            =   "txtVersion"
      Top             =   3960
      Width           =   615
   End
   Begin VB.CommandButton cmdSearchDel 
      Caption         =   "删除"
      Height          =   255
      Left            =   960
      TabIndex        =   12
      Top             =   3720
      Width           =   615
   End
   Begin VB.CommandButton cmdSearchAdd 
      Caption         =   "增加"
      Height          =   255
      Left            =   240
      TabIndex        =   11
      Top             =   3720
      Width           =   615
   End
   Begin VB.CommandButton cmdSearchCountModify 
      Caption         =   "修改"
      Height          =   255
      Left            =   1320
      TabIndex        =   10
      Top             =   6360
      Width           =   615
   End
   Begin VB.CommandButton cmdSearchCountDel 
      Caption         =   "删除"
      Height          =   255
      Left            =   720
      TabIndex        =   9
      Top             =   6360
      Width           =   615
   End
   Begin VB.CommandButton cmdSearchCountAdd 
      Caption         =   "增加"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   6360
      Width           =   615
   End
   Begin VB.TextBox txtSearchLen 
      Height          =   285
      Left            =   3720
      TabIndex        =   5
      Text            =   "txtSearchLen"
      Top             =   6360
      Width           =   855
   End
   Begin VB.TextBox txtSearchPos 
      Height          =   285
      Left            =   2400
      TabIndex        =   4
      Text            =   "txtSearchPos"
      Top             =   6360
      Width           =   735
   End
   Begin VB.CommandButton cmdSaveAs 
      Caption         =   "另存为"
      Height          =   255
      Left            =   5640
      TabIndex        =   3
      Top             =   120
      Width           =   735
   End
   Begin VB.CommandButton cmdSave 
      Caption         =   "保存"
      Height          =   255
      Left            =   4800
      TabIndex        =   2
      Top             =   120
      Width           =   735
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "打开"
      Height          =   255
      Left            =   3960
      TabIndex        =   1
      Top             =   120
      Width           =   735
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000004&
      Height          =   285
      Left            =   120
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   120
      Width           =   3735
   End
   Begin ComctlLib.ListView lvwSearchCount 
      Height          =   1575
      Left            =   120
      TabIndex        =   42
      Top             =   4680
      Width           =   4455
      _ExtentX        =   7858
      _ExtentY        =   2778
      View            =   3
      LabelEdit       =   1
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
         Text            =   "位置"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   1
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "大小"
         Object.Width           =   2540
      EndProperty
   End
   Begin ComctlLib.ListView lvwInfo 
      Height          =   1575
      Left            =   4800
      TabIndex        =   43
      Top             =   480
      Width           =   4695
      _ExtentX        =   8281
      _ExtentY        =   2778
      View            =   3
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      _Version        =   327682
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   8
      BeginProperty ColumnHeader(1) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "序号"
         Object.Width           =   882
      EndProperty
      BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   1
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "类型"
         Object.Width           =   882
      EndProperty
      BeginProperty ColumnHeader(3) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   2
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "厂商"
         Object.Width           =   1058
      EndProperty
      BeginProperty ColumnHeader(4) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   3
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "WEB"
         Object.Width           =   1058
      EndProperty
      BeginProperty ColumnHeader(5) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   4
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "标识"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(6) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   5
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "数据位置"
         Object.Width           =   1129
      EndProperty
      BeginProperty ColumnHeader(7) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   6
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "指令位置"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(8) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   7
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "指令串"
         Object.Width           =   2540
      EndProperty
   End
   Begin ComctlLib.ListView lvwVersion 
      Height          =   1335
      Left            =   4680
      TabIndex        =   44
      Top             =   3960
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   2355
      View            =   3
      LabelEdit       =   1
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
         Text            =   "版本"
         Object.Width           =   1129
      EndProperty
      BeginProperty ColumnHeader(2) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         SubItemIndex    =   1
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "描述"
         Object.Width           =   2540
      EndProperty
   End
   Begin ComctlLib.ListView lvwPEString 
      Height          =   1335
      Left            =   4680
      TabIndex        =   45
      Top             =   5400
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   2355
      View            =   3
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      _Version        =   327682
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   1
      BeginProperty ColumnHeader(1) {0713E8C7-850A-101B-AFC0-4210102A8DA7} 
         Key             =   ""
         Object.Tag             =   ""
         Text            =   "PE搜索串"
         Object.Width           =   2540
      EndProperty
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   "描述"
      Height          =   195
      Left            =   0
      TabIndex        =   50
      Top             =   4200
      Width           =   360
   End
   Begin VB.Label Label13 
      Caption         =   "指令串"
      Height          =   255
      Left            =   6480
      TabIndex        =   35
      Top             =   3480
      Width           =   615
   End
   Begin VB.Label Label11 
      AutoSize        =   -1  'True
      Caption         =   "指令位置"
      Height          =   195
      Left            =   4800
      TabIndex        =   34
      Top             =   3480
      Width           =   720
   End
   Begin VB.Label Label10 
      Caption         =   "数据位置"
      Height          =   255
      Left            =   7680
      TabIndex        =   32
      Top             =   3120
      Width           =   735
   End
   Begin VB.Label Label9 
      Caption         =   "文档头部标识"
      Height          =   255
      Left            =   4800
      TabIndex        =   30
      Top             =   3120
      Width           =   1335
   End
   Begin VB.Label Label8 
      AutoSize        =   -1  'True
      Caption         =   "文档类型(RS_DWORD 引擎定义ID)"
      Height          =   195
      Left            =   4800
      TabIndex        =   24
      Top             =   2400
      Width           =   2700
   End
   Begin VB.Label Label7 
      AutoSize        =   -1  'True
      Caption         =   "描述"
      Height          =   195
      Left            =   7800
      TabIndex        =   16
      Top             =   3960
      Width           =   360
   End
   Begin VB.Label Label6 
      AutoSize        =   -1  'True
      Caption         =   "版本"
      Height          =   195
      Left            =   6600
      TabIndex        =   15
      Top             =   3960
      Width           =   360
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "Web"
      Height          =   195
      Left            =   6840
      TabIndex        =   14
      Top             =   2760
      Width           =   345
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "厂商"
      Height          =   195
      Left            =   4800
      TabIndex        =   13
      Top             =   2760
      Width           =   360
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "大小"
      Height          =   195
      Left            =   3240
      TabIndex        =   7
      Top             =   6360
      Width           =   360
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "位置"
      Height          =   195
      Left            =   1920
      TabIndex        =   6
      Top             =   6360
      Width           =   360
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Any) As Long
Private Const LVM_SETEXTENDEDLISTVIEWSTYLE As Long = &H1000& + 54
Private Const LVS_EX_FULLROWSELECT As Long = &H20&           'applies to report mode only
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)





Private Type udtSearch
    search_id As Long
    search_count As Long
    search_pos() As Long
    search_len() As Long
    search_desc_len As Long
    search_desc As String
    reserve_count As Long
End Type

Private Type udtInfo
    id As Long          '这个id是标识一条内部使用。 不入数据库
    search_id As Long
    archive_type As Long
    corp_len As Long
    corp  As String
    corpweb_len As Long
    corpweb  As String
    VersionCount  As Long
    Version() As Long
    VersionString_len() As Long
    VersionString() As String
    PEStringCount  As Long
    PEStringLen() As Long
    PEString() As String
    ArchiveStringLen  As Long
    ArchiveString As String
    SFXPos As Long
    IPos As Long
    ILen As Long
    IString As String
End Type

Private searchs() As udtSearch
Private infos() As udtInfo
Private max_info_id As Long





'////////////////////////////////////////////////////////
'
'               FORM  function
'
'////////////////////////////////////////////////////////
Private Sub Form_Load()
    ClearForm
    SendMessage lvwSearch.hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT
    SendMessage lvwSearchCount.hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT
    SendMessage lvwInfo.hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT
    SendMessage lvwVersion.hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT
    SendMessage lvwPEString.hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT
End Sub



'////////////////////////////////////////////////////////
'
'               command  function
'
'////////////////////////////////////////////////////////


'打开
Private Sub cmdOpen_Click()
On Error GoTo errHandle
    Dim file As String
    With CommonDialog1
        .CancelError = True
        .Filter = "Ext.def|*.def|所有文件(*.*)|*.*"
lblOpen:
        .ShowOpen
        file = .FileName
        If Dir(file) = "" Then GoTo lblOpen
        txtFile.Text = file
    End With
    Call ClearForm
    Erase searchs
    Erase infos
    
    
    Dim byts(6) As Byte
    Dim bytall() As Byte
    Dim i As Long, j As Long
    Dim search_cnt As Long, info_cnt As Long
    search_cnt = 0: info_cnt = 0: max_info_id = 0
    
    Dim fileNumber As Integer
    fileNumber = FreeFile

    Open file For Binary As fileNumber
        '--get file mark
        Get fileNumber, , byts
        If byts(0) <> Asc("R") Or byts(1) <> Asc("S") _
            Or byts(2) <> Asc("E") Or byts(3) <> Asc("X") _
            Or byts(4) <> Asc("T") Or byts(5) <> 0 Then
            MsgBox "文件格式错误", vbCritical
            Close fileNumber
            Exit Sub
        End If

        '--get file CRC
        ReDim bytall(((LOF(fileNumber) + 3) \ 4) * 4 - 1)
        Seek fileNumber, 1
        Get fileNumber, , bytall
        Dim lngCRC As Long
        If Not CRC(bytall, lngCRC) Then
            If MsgBox("文件CRC错误,是否继续?", vbCritical + vbQuestion + vbYesNo) = vbNo Then
                Close fileNumber
                Exit Sub
            Else
                StatusBar1.Panels(3).Text = "CRC错误:" & Hex(lngCRC)
            End If
        Else
            StatusBar1.Panels(3).Text = "CRC正确:" & Hex(lngCRC)
        End If
        
        'get file date
        Dim lngFileDate As Long
        lngFileDate = GetLong(bytall, 8)
        StatusBar1.Panels(2).Text = "文件日期:" & DateAdd("s", lngFileDate, "1970-1-1 00:00:00")
        
        'parse data
        i = 16
        Do While i <= LOF(fileNumber) - 1
            Dim ibak As Long
            ibak = i
        
            If bytall(i) = &H11 Then        'search
                i = i + 5
                Dim search As udtSearch
                search.search_id = GetLong(bytall, i)
                search.search_count = GetLong(bytall, i)
                ReDim search.search_pos(search.search_count)
                ReDim search.search_len(search.search_count)
                For j = 1 To search.search_count
                    search.search_pos(j) = GetLong(bytall, i)
                    search.search_len(j) = GetLong(bytall, i)
                Next j
                search.search_desc_len = GetLong(bytall, i)
                search.search_desc = GetString(bytall, i, search.search_desc_len, True)
                search.reserve_count = GetLong(bytall, i)
                If SearchSearchById(search.search_id, search) < 0 Then 'id重复不记录
                    ReDim Preserve searchs(search_cnt)
                    searchs(search_cnt) = search
                    search_cnt = search_cnt + 1
                    Call LvwAddSearch(search)
                End If
            ElseIf bytall(i) = &H12 Then    'info
                i = i + 5
                Dim info As udtInfo
                info.search_id = GetLong(bytall, i)
                info.archive_type = GetLong(bytall, i)
                info.corp_len = GetLong(bytall, i)
                info.corp = GetString(bytall, i, info.corp_len, True)
                info.corpweb_len = GetLong(bytall, i)
                info.corpweb = GetString(bytall, i, info.corpweb_len, True)
                info.VersionCount = GetLong(bytall, i)
                ReDim info.Version(info.VersionCount)
                ReDim info.VersionString_len(info.VersionCount)
                ReDim info.VersionString(info.VersionCount)
                For j = 1 To info.VersionCount
                    info.Version(j) = GetLong(bytall, i)
                    info.VersionString_len(j) = GetLong(bytall, i)
                    info.VersionString(j) = GetString(bytall, i, info.VersionString_len(j), True)
                Next j
                info.PEStringCount = GetLong(bytall, i)
                ReDim info.PEStringLen(info.PEStringCount)
                ReDim info.PEString(info.PEStringCount)
                For j = 1 To info.PEStringCount
                    info.PEStringLen(j) = GetLong(bytall, i)
                    info.PEString(j) = GetString(bytall, i, info.PEStringLen(j))
                Next j
                info.ArchiveStringLen = GetLong(bytall, i)
                info.ArchiveString = GetString(bytall, i, info.ArchiveStringLen)
                info.SFXPos = GetLong(bytall, i)
                info.IPos = GetLong(bytall, i)
                info.ILen = GetLong(bytall, i)
                info.IString = GetString(bytall, i, info.ILen)
                max_info_id = max_info_id + 1
                info.id = max_info_id
                ReDim Preserve infos(info_cnt)
                infos(info_cnt) = info
                info_cnt = info_cnt + 1
            Else        'pass
                
            End If
            i = ibak + GetLong(bytall, ibak + 1)
        Loop
    Close fileNumber
    If lvwSearch.ListItems.Count > 0 Then
        lvwSearch.SelectedItem = lvwSearch.ListItems(1)
        lvwSearch_ItemClick lvwSearch.SelectedItem
    End If
    
    Exit Sub
errHandle:
    If Err.Number <> 32755 Then MsgBox Err.Description 'user select cancel
End Sub

'保存
Private Sub cmdSave_Click()
    Call SaveFile(txtFile)
End Sub


'另存为
Private Sub cmdSaveAs_Click()
    On Error GoTo errHandle
    With CommonDialog1
        .CancelError = True
        .Filter = "Ext.def|*.def|所有文件(*.*)|*.*"
        .FileName = "Ext.def"
lblSave:
        .ShowSave
        If Dir(.FileName) <> "" Then
            If MsgBox("文件已存在,是否覆盖?", vbQuestion + vbYesNo) = vbNo Then GoTo lblSave
        End If
        Call SaveFile(.FileName)
    End With
    
    Exit Sub
errHandle:
    If Err.Number <> 32755 Then MsgBox Err.Description 'user select cancel
End Sub

Private Sub SaveFile(file As String)
    On Error GoTo errHandle
    Dim byt() As Byte
    Dim one As Byte
    Dim dword As Long
    Dim intFile As Integer
    Dim i As Long, j As Long
    Kill file
    
    Dim retinfos() As udtInfo
    For i = 0 To ArrCntSearch(searchs) - 1
        If SearchInfosById(searchs(i).search_id, retinfos) Then
            searchs(i).reserve_count = UBound(retinfos) + 1
        Else
            searchs(i).reserve_count = 0
        End If
        
    Next i
    
    For i = 0 To ArrCntInfo(infos) - 1
        
    Next i
    
    intFile = FreeFile()
    Open file For Binary As intFile
        'mark
        ReDim byt(5)
        byt(0) = Asc("R"): byt(1) = Asc("S"): byt(2) = Asc("E")
        byt(3) = Asc("X"): byt(4) = Asc("T"): byt(5) = 0
        Put intFile, , byt
        'head
        Seek intFile, 17
        one = &H10: Put intFile, , one
        dword = 13: Put intFile, , dword
        dword = ArrCntSearch(searchs): Put intFile, , dword
        dword = ArrCntInfo(infos): Put intFile, , dword
        'search
        For i = 0 To ArrCntSearch(searchs) - 1
            one = &H11: Put intFile, , one
            dword = 21 + searchs(i).search_count * 8 + searchs(i).search_desc_len
            Put intFile, , dword
            dword = searchs(i).search_id: Put intFile, , dword
            dword = searchs(i).search_count: Put intFile, , dword
            For j = 1 To searchs(i).search_count
                dword = searchs(i).search_pos(j): Put intFile, , dword
                dword = searchs(i).search_len(j): Put intFile, , dword
            Next j
            dword = searchs(i).search_desc_len: Put intFile, , dword
            If dword > 0 Then
                Call SetString(byt, searchs(i).search_desc, True): Put intFile, , byt
            End If
            dword = searchs(i).reserve_count: Put intFile, , dword
        Next i
        'info
        For i = 0 To ArrCntInfo(infos) - 1
            one = &H12: Put intFile, , one
            dword = 45
            For j = 1 To infos(i).VersionCount
                dword = dword + 8 + infos(i).VersionString_len(j)
            Next j
            For j = 1 To infos(i).PEStringCount
                dword = dword + 4 + infos(i).PEStringLen(j)
            Next j
            dword = dword + infos(i).corp_len
            dword = dword + infos(i).corpweb_len
            dword = dword + infos(i).ArchiveStringLen
            dword = dword + infos(i).ILen
            Put intFile, , dword
            dword = infos(i).search_id: Put intFile, , dword
            dword = infos(i).archive_type: Put intFile, , dword
            dword = infos(i).corp_len: Put intFile, , dword
            If dword > 0 Then
                Call SetString(byt, infos(i).corp, True): Put intFile, , byt
            End If
            dword = infos(i).corpweb_len: Put intFile, , dword
            If dword > 0 Then
                Call SetString(byt, infos(i).corpweb, True): Put intFile, , byt
            End If
            dword = infos(i).VersionCount: Put intFile, , dword
            For j = 1 To infos(i).VersionCount
                dword = infos(i).Version(j): Put intFile, , dword
                dword = infos(i).VersionString_len(j): Put intFile, , dword
                If dword > 0 Then
                    Call SetString(byt, infos(i).VersionString(j), True): Put intFile, , byt
                End If
            Next j
            dword = infos(i).PEStringCount: Put intFile, , dword
            For j = 1 To infos(i).PEStringCount
                dword = infos(i).PEStringLen(j): Put intFile, , dword
                If dword > 0 Then
                    Call SetString(byt, infos(i).PEString(j)): Put intFile, , byt
                End If
            Next j
            dword = infos(i).ArchiveStringLen: Put intFile, , dword
            If dword > 0 Then
                Call SetString(byt, infos(i).ArchiveString): Put intFile, , byt
            End If
            dword = infos(i).SFXPos: Put intFile, , dword
            dword = infos(i).IPos: Put intFile, , dword
            dword = infos(i).ILen: Put intFile, , dword
            If dword > 0 Then
                Call SetString(byt, infos(i).IString): Put intFile, , byt
            End If
        Next i
        'time && crc
        Seek intFile, 9
        dword = DateDiff("s", "1970-1-1 00:00:00", Now): Put intFile, , dword
        Seek intFile, 13
        dword = 0: Put intFile, , dword
    Close intFile
    Call CRCFile(file)
    MsgBox "保存成功"
    
    Exit Sub
errHandle:
    MsgBox Err.Number & vbCrLf & Err.Description
End Sub


'search add
Private Sub cmdSearchAdd_Click()
    Dim search As udtSearch
    '数据库中一定是顺序的Search,并且从1开始!!
    ReDim Preserve searchs(ArrCntSearch(searchs))
    searchs(UBound(searchs)).search_id = ArrCntSearch(searchs)
    ReDim searchs(UBound(searchs)).search_len(0)
    ReDim searchs(UBound(searchs)).search_pos(0)
    Call LvwAddSearch(searchs(UBound(searchs)))
    lvwSearch.SelectedItem = lvwSearch.ListItems(lvwSearch.ListItems.Count)
    Call lvwSearch_ItemClick(lvwSearch.SelectedItem)
End Sub

'search del
Private Sub cmdSearchDel_Click()
    If lvwSearch.ListItems.Count = 0 Then Exit Sub
    If lvwSearch.SelectedItem Is Nothing Then MsgBox "请选择一个搜索方法": Exit Sub
    'If MsgBox("确实要删除此搜索方法吗?", vbQuestion + vbYesNo) = vbNo Then Exit Sub
    '这个方法不存在了, 那么所有的跟这个方法有关的info也必须删除.
    '删除一个info后, 必须让后面的info后移
    '最后遍历一次info. 让所有的search_id比这个方法大的都减1,同时重新生成id
    Dim id As Long
    id = Val(lvwSearch.SelectedItem.Text)
    Call SearchDel(id)
    Call InfoDelSearch(id)
    Call ClearFormNolvw
    If Not lvwSearch.SelectedItem Is Nothing Then
        Call lvwSearch_ItemClick(lvwSearch.SelectedItem)
    End If
End Sub

'search modify
Private Sub cmdSearchModify_Click()
    If lvwSearch.ListItems.Count = 0 Then Exit Sub
    If lvwSearch.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, search As udtSearch
    index = SearchSearchById(lvwSearch.SelectedItem.Text, search)
    If index >= 0 Then
        searchs(index).search_desc = Trim(txtSearchDesc.Text)
        searchs(index).search_desc_len = LenB(StrConv(Trim(txtSearchDesc.Text), vbFromUnicode))
    End If
    lvwSearch.SelectedItem.SubItems(1) = Trim(txtSearchDesc.Text)
End Sub

'search count add
Private Sub cmdSearchCountAdd_Click()
    If lvwSearch.SelectedItem Is Nothing Then Exit Sub
    Dim search As udtSearch
    Dim index As Long, lstX As ListItem
    index = SearchSearchById(lvwSearch.SelectedItem.Text, search)
    If index >= 0 Then
        searchs(index).search_count = searchs(index).search_count + 1
        ReDim Preserve searchs(index).search_pos(UBound(searchs(index).search_pos) + 1)
        ReDim Preserve searchs(index).search_len(UBound(searchs(index).search_len) + 1)
        Set lstX = lvwSearchCount.ListItems.Add(, , "")
        lvwSearchCount.SelectedItem = lstX
        Call lvwSearchCount_ItemClick(lvwSearchCount.SelectedItem)
    End If
End Sub

'search count del
Private Sub cmdSearchCountDel_Click()
    If lvwSearch.SelectedItem Is Nothing Then Exit Sub
    If lvwSearchCount.SelectedItem Is Nothing Then Exit Sub
    Dim search As udtSearch
    Dim index As Long, lstX As ListItem, i As Long, j As Long
    index = SearchSearchById(lvwSearch.SelectedItem.Text, search)
    If index >= 0 Then
        'If MsgBox("确定删除?", vbYesNo + vbQuestion) = vbNo Then Exit Sub
        For i = 1 To searchs(index).search_count
            If searchs(index).search_pos(i) = Val(lvwSearchCount.SelectedItem.Text) And _
                searchs(index).search_len(i) = Val(lvwSearchCount.SelectedItem.SubItems(1)) Then
                For j = i To searchs(index).search_count - 1
                    searchs(index).search_pos(j) = searchs(index).search_pos(j + 1)
                    searchs(index).search_len(j) = searchs(index).search_len(j + 1)
                Next j
                ReDim Preserve searchs(index).search_len(searchs(index).search_count - 1)
                ReDim Preserve searchs(index).search_pos(searchs(index).search_count - 1)
                searchs(index).search_count = searchs(index).search_count - 1
                Exit For
            End If
        Next i
        lvwSearchCount.ListItems.Remove lvwSearchCount.SelectedItem.index
        Call ClearSearchCountNolvw
        If Not lvwSearchCount.SelectedItem Is Nothing Then
            Call lvwSearchCount_ItemClick(lvwSearchCount.SelectedItem)
        End If
    End If
End Sub

'search count modify
Private Sub cmdSearchCountModify_Click()
    If lvwSearch.SelectedItem Is Nothing Then Exit Sub
    If lvwSearchCount.SelectedItem Is Nothing Then Exit Sub
    Dim search As udtSearch, index As Long, i As Long
    index = SearchSearchById(lvwSearch.SelectedItem.Text, search)
    If index >= 0 Then
        For i = 1 To searchs(index).search_count
            If searchs(index).search_pos(i) = Val(lvwSearchCount.SelectedItem.Text) And _
                searchs(index).search_len(i) = Val(lvwSearchCount.SelectedItem.SubItems(1)) Then
                searchs(index).search_pos(i) = Val(txtSearchPos.Text)
                searchs(index).search_len(i) = Val(txtSearchLen.Text)
                Exit For
            End If
        Next i
        lvwSearchCount.SelectedItem.Text = Val(txtSearchPos.Text)
        lvwSearchCount.SelectedItem.SubItems(1) = Val(txtSearchLen.Text)
    End If
End Sub


'info add
Private Sub cmdInfoAdd_Click()
    If lvwSearch.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long
    index = ArrCntInfo(infos)
    ReDim Preserve infos(index)
    infos(index).id = index + 1
    infos(index).search_id = lvwSearch.SelectedItem.Text
    ReDim infos(index).Version(0)
    ReDim infos(index).VersionString(0)
    ReDim infos(index).VersionString_len(0)
    ReDim infos(index).PEString(0)
    ReDim infos(index).PEStringLen(0)
    Call LvwAddInfo(infos(index))
    lvwInfo.SelectedItem = lvwInfo.ListItems(lvwInfo.ListItems.Count)
    lvwInfo_ItemClick lvwInfo.SelectedItem
End Sub

'info del
Private Sub cmdInfoDel_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    Call InfoDel(lvwInfo.SelectedItem.Text)
    If Not lvwInfo.SelectedItem Is Nothing Then
        lvwInfo_ItemClick lvwInfo.SelectedItem
    End If
End Sub

'info modify
Private Sub cmdInfoModify_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long
    index = lvwInfo.SelectedItem.Text - 1
    infos(index).archive_type = Val(txtArchiveType.Text)
    infos(index).corp_len = LenB(StrConv(Trim(txtCorp.Text), vbFromUnicode))
    infos(index).corp = Trim(txtCorp.Text)
    infos(index).corpweb_len = LenB(StrConv(Trim(txtWeb.Text), vbFromUnicode))
    infos(index).corpweb = Trim(txtWeb.Text)
    infos(index).ArchiveStringLen = LenB(StringHex(Trim(txtArchiveString.Text)))
    infos(index).ArchiveString = StringHex(Trim(txtArchiveString.Text))
    infos(index).SFXPos = Val(txtSFXPos.Text)
    infos(index).IPos = Val(txtIPos.Text)
    infos(index).ILen = LenB(StringHex(Trim(txtIString.Text)))
    infos(index).IString = StringHex(Trim(txtIString.Text))
    '---
    lvwInfo.SelectedItem.SubItems(1) = Val(txtArchiveType.Text)
    lvwInfo.SelectedItem.SubItems(2) = Trim(txtCorp.Text)
    lvwInfo.SelectedItem.SubItems(3) = Trim(txtWeb.Text)
    lvwInfo.SelectedItem.SubItems(4) = Trim(txtArchiveString.Text)
    lvwInfo.SelectedItem.SubItems(5) = Val(txtSFXPos.Text)
    lvwInfo.SelectedItem.SubItems(6) = Val(txtIPos.Text)
    lvwInfo.SelectedItem.SubItems(7) = Trim(txtIString.Text)
End Sub

'version add
Private Sub cmdVersionAdd_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, index2 As Long, lstX As ListItem
    index = lvwInfo.SelectedItem.Text - 1
    infos(index).VersionCount = infos(index).VersionCount + 1
    index2 = UBound(infos(index).Version) + 1
    ReDim Preserve infos(index).Version(index2)
    ReDim Preserve infos(index).VersionString(index2)
    ReDim Preserve infos(index).VersionString_len(index2)
    Set lstX = lvwVersion.ListItems.Add(, , "")
    Set lvwVersion.SelectedItem = lstX
    lvwVersion_ItemClick lvwVersion.SelectedItem
End Sub

'version del
Private Sub cmdVersionDel_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    If lvwVersion.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, i As Long, j As Long, lstX As ListItem
    index = lvwInfo.SelectedItem.Text - 1
    
    For i = 1 To infos(index).VersionCount
        If infos(index).Version(i) = Val(lvwVersion.SelectedItem.Text) And _
            infos(index).VersionString(i) = lvwVersion.SelectedItem.SubItems(1) Then
            For j = i To infos(index).VersionCount - 1
                infos(index).Version(j) = infos(index).Version(j + 1)
                infos(index).VersionString_len(j) = infos(index).VersionString_len(j + 1)
                infos(index).VersionString(j) = infos(index).VersionString(j + 1)
            Next j
            Exit For
        End If
    Next i
    infos(index).VersionCount = infos(index).VersionCount - 1
    ReDim Preserve infos(index).Version(infos(index).VersionCount)
    ReDim Preserve infos(index).VersionString(infos(index).VersionCount)
    ReDim Preserve infos(index).VersionString_len(infos(index).VersionCount)
    lvwVersion.ListItems.Remove lvwVersion.SelectedItem.index
    If Not lvwVersion.SelectedItem Is Nothing Then
        lvwVersion_ItemClick lvwVersion.SelectedItem
    End If
End Sub

'version modify
Private Sub cmdVersionModify_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    If lvwVersion.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, i As Long, j As Long, lstX As ListItem
    index = lvwInfo.SelectedItem.Text - 1
    For i = 1 To infos(index).VersionCount
        If infos(index).Version(i) = Val(lvwVersion.SelectedItem.Text) And _
            infos(index).VersionString(i) = lvwVersion.SelectedItem.SubItems(1) Then
            infos(index).Version(i) = Val(txtVersion)
            infos(index).VersionString_len(i) = LenB(StrConv(Trim(txtVersionDesc.Text), vbFromUnicode))
            infos(index).VersionString(i) = Trim(txtVersionDesc.Text)
            Exit For
        End If
    Next i
    lvwVersion.SelectedItem.Text = Val(txtVersion.Text)
    lvwVersion.SelectedItem.SubItems(1) = Trim(txtVersionDesc.Text)
End Sub

'archive select
Private Sub cmdTypeSelect_Click()
    Form2.Show vbModal, Me
End Sub

'PEString add
Private Sub cmdPEStringAdd_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, index2 As Long, lstX As ListItem
    index = lvwInfo.SelectedItem.Text - 1
    infos(index).PEStringCount = infos(index).PEStringCount + 1
    index2 = UBound(infos(index).PEString) + 1
    ReDim Preserve infos(index).PEString(index2)
    ReDim Preserve infos(index).PEStringLen(index2)
    Set lstX = lvwPEString.ListItems.Add(, , "")
    Set lvwPEString.SelectedItem = lstX
    lvwPEString_ItemClick lvwPEString.SelectedItem
End Sub

'PEString del
Private Sub cmdPEStringDel_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    If lvwPEString.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, i As Long, j As Long, lstX As ListItem
    index = lvwInfo.SelectedItem.Text - 1
    
    For i = 1 To infos(index).PEStringCount
        If infos(index).PEString(i) = lvwPEString.SelectedItem.Text Then
            For j = i To infos(index).PEStringCount - 1
                infos(index).PEStringLen(j) = infos(index).PEStringLen(j + 1)
                infos(index).PEString(j) = infos(index).PEString(j + 1)
            Next j
            Exit For
        End If
    Next i
    infos(index).PEStringCount = infos(index).PEStringCount - 1
    ReDim Preserve infos(index).PEStringLen(infos(index).PEStringCount)
    ReDim Preserve infos(index).PEString(infos(index).PEStringCount)
    lvwPEString.ListItems.Remove lvwPEString.SelectedItem.index
    If Not lvwPEString.SelectedItem Is Nothing Then
        lvwPEString_ItemClick lvwPEString.SelectedItem
    End If
End Sub


'PEString modify
Private Sub cmdPEStringModify_Click()
    If lvwInfo.SelectedItem Is Nothing Then Exit Sub
    If lvwPEString.SelectedItem Is Nothing Then Exit Sub
    Dim index As Long, i As Long, j As Long, lstX As ListItem
    index = lvwInfo.SelectedItem.Text - 1
    For i = 1 To infos(index).PEStringCount
        If infos(index).PEString(i) = StringHex(lvwPEString.SelectedItem.Text) Then
            infos(index).PEStringLen(i) = LenB(StringHex(Trim(txtPEString.Text)))
            infos(index).PEString(i) = StringHex(Trim(txtPEString.Text))
            Exit For
        End If
    Next i
    lvwPEString.SelectedItem.Text = Trim(txtPEString.Text)
End Sub

'////////////////////////////////////////////////////////
'
'               lvw  function
'
'////////////////////////////////////////////////////////
Private Sub lvwSearch_ItemClick(ByVal Item As ComctlLib.ListItem)
    Call ClearFormNolvw
    Dim search As udtSearch, i As Long, lstX As ListItem
    If SearchSearchById(Item.Text, search) >= 0 Then
        For i = 1 To search.search_count
            Set lstX = lvwSearchCount.ListItems.Add(, , search.search_pos(i))
            lstX.SubItems(1) = search.search_len(i)
        Next i
        If search.search_count > 0 Then
            lvwSearchCount.SelectedItem = lvwSearchCount.ListItems(1)
            lvwSearchCount_ItemClick lvwSearchCount.SelectedItem
        End If
        txtSearchDesc.Text = search.search_desc
    End If
    
    Dim infos_() As udtInfo
    If SearchInfosById(Item.Text, infos_) Then
        For i = 0 To UBound(infos_)
            Call LvwAddInfo(infos_(i))
        Next i
        lvwInfo_ItemClick lvwInfo.ListItems(1)
    End If
    
End Sub

'search 中关于search pos， len的 lvw
Private Sub lvwSearchCount_ItemClick(ByVal Item As ComctlLib.ListItem)
    Call ClearSearchCountNolvw
    txtSearchPos.Text = Item.Text
    txtSearchLen = Item.SubItems(1)
End Sub

'info lvw
Private Sub lvwInfo_ItemClick(ByVal Item As ComctlLib.ListItem)
    Dim info As udtInfo
    Dim lstX As ListItem
    Dim i As Long
    Call ClearInfoNolvw
    If SearchInfoById(Item.Text, info) Then
        For i = 1 To info.VersionCount
            Set lstX = lvwVersion.ListItems.Add(, , info.Version(i))
            lstX.SubItems(1) = info.VersionString(i)
        Next i
        If info.VersionCount > 0 Then
            lvwVersion_ItemClick lvwVersion.ListItems(1)
        End If
        '--
        For i = 1 To info.PEStringCount
            Set lstX = lvwPEString.ListItems.Add(, , HexString(info.PEString(i)))
        Next i
        If info.PEStringCount > 0 Then
            lvwPEString_ItemClick lvwPEString.ListItems(1)
        End If
        '--
        txtArchiveType.Text = info.archive_type
        txtCorp.Text = info.corp
        txtWeb.Text = info.corpweb
        txtArchiveString.Text = HexString(info.ArchiveString)
        txtSFXPos.Text = info.SFXPos
        txtIPos.Text = info.IPos
        txtIString.Text = HexString(info.IString)
    End If
End Sub

'version lvw
Private Sub lvwVersion_ItemClick(ByVal Item As ComctlLib.ListItem)
    ClearVersionNolvw
    txtVersion.Text = Item.Text
    txtVersionDesc.Text = Item.SubItems(1)
End Sub

'PEString lvw
Private Sub lvwPEString_ItemClick(ByVal Item As ComctlLib.ListItem)
    ClearPeStringNolvw
    txtPEString.Text = Item.Text
End Sub



Private Sub LvwAddSearch(search As udtSearch)
    Dim lstX As ListItem
    Set lstX = lvwSearch.ListItems.Add(, , search.search_id)
    lstX.SubItems(1) = search.search_desc
End Sub

Private Sub LvwAddInfo(info As udtInfo)
    Dim lstX As ListItem
    Set lstX = lvwInfo.ListItems.Add(, , info.id)
    lstX.SubItems(1) = info.archive_type
    lstX.SubItems(2) = info.corp
    lstX.SubItems(3) = info.corpweb
    lstX.SubItems(4) = HexString(info.ArchiveString)
    lstX.SubItems(5) = info.SFXPos
    lstX.SubItems(6) = info.IPos
    lstX.SubItems(7) = HexString(info.IString)
End Sub





'///////////////////////////////////////////////////////
'
'               Text Lost Focus
'
'///////////////////////////////////////////////////////
Private Sub txtSearchDesc_LostFocus()
    Call cmdSearchModify_Click
End Sub

Private Sub txtSearchPos_LostFocus()
    Call cmdSearchCountModify_Click
End Sub
Private Sub txtSearchLen_LostFocus()
    cmdSearchCountModify_Click
End Sub

Private Sub txtArchiveType_LostFocus()
    Call cmdInfoModify_Click
End Sub
Private Sub txtCorp_LostFocus()
    Call cmdInfoModify_Click
End Sub
Private Sub txtWeb_LostFocus()
    Call cmdInfoModify_Click
End Sub
Private Sub txtArchiveString_LostFocus()
    Call cmdInfoModify_Click
End Sub
Private Sub txtSFXPos_LostFocus()
    Call cmdInfoModify_Click
End Sub
Private Sub txtIPos_LostFocus()
    Call cmdInfoModify_Click
End Sub
Private Sub txtIString_LostFocus()
    Call cmdInfoModify_Click
End Sub

Private Sub txtVersion_LostFocus()
    Call cmdVersionModify_Click
End Sub
Private Sub txtVersionDesc_LostFocus()
    Call cmdVersionModify_Click
End Sub

Private Sub txtPEString_LostFocus()
    Call cmdPEStringModify_Click
End Sub

'////////////////////////////////////////////////////////
'
'               search struct function
'
'////////////////////////////////////////////////////////
Private Sub SearchDel(id As Long)
    On Error GoTo errHandle
    Dim i As Long, j As Long, lstX As ListItem, lstX2 As ListItem
    For i = 0 To ArrCntSearch(searchs) - 1
        If searchs(i).search_id = id Then
            For j = i To ArrCntSearch(searchs) - 2
                searchs(j) = searchs(j + 1)
                searchs(j).search_id = searchs(j).search_id - 1
            Next j
        End If
        If ArrCntSearch(searchs) > 1 Then
            ReDim Preserve searchs(UBound(searchs) - 1)
        Else
            Erase searchs
        End If
        Exit For
    Next i
        
    For i = 1 To lvwSearch.ListItems.Count
        Set lstX = lvwSearch.ListItems(i)
        If lstX.Text = id Then
            For j = i To lvwSearch.ListItems.Count - 1
                Set lstX = lvwSearch.ListItems(j)
                Set lstX2 = lvwSearch.ListItems(j + 1)
                lstX.Text = lstX2.Text - 1
                lstX.SubItems(1) = lstX2.SubItems(1)
            Next j
            lvwSearch.ListItems.Remove lvwSearch.ListItems.Count
            Exit For
        End If
    Next i
errHandle:
End Sub

Private Sub InfoDelSearch(id_search As Long)
    On Error GoTo errHandle
    Dim i As Long, j As Long
    j = 0
    For i = 0 To ArrCntInfo(infos) - 1
        Do
            If j < ArrCntInfo(infos) Then
                If infos(j).search_id = id_search Then
                    j = j + 1
                Else
                    Exit Do
                End If
            Else
                Exit Do
            End If
        Loop
                
        If j = ArrCntInfo(infos) Then
            If i > 0 Then
                ReDim Preserve infos(i - 1)
            Else
                Erase infos
            End If
            Exit For
        Else
            If i <> j Then infos(i) = infos(j)
            j = j + 1
        End If
    Next i
    
    For i = 0 To ArrCntInfo(infos) - 1
        If infos(i).search_id > id_search Then infos(i).search_id = infos(i).search_id - 1
        infos(i).id = i
    Next i
errHandle:
End Sub


Private Sub InfoDel(id As Long)
    On Error GoTo errHandle
    Dim i As Long, j As Long
    Dim lstX As ListItem, lstX2 As ListItem
    If id > ArrCntInfo(infos) Then Exit Sub
    If id = 0 Then Exit Sub
    For i = id - 1 To ArrCntInfo(infos) - 2
        infos(i) = infos(i + 1)
        infos(i).id = infos(i).id - 1
    Next i
    If ArrCntInfo(infos) > 1 Then
        ReDim Preserve infos(UBound(infos) - 1)
    Else
        Erase infos
    End If
    '---
    For i = 1 To lvwInfo.ListItems.Count
        Set lstX = lvwInfo.ListItems(i)
        If lstX.Text = id Then
            For j = i To lvwInfo.ListItems.Count - 1
                Set lstX = lvwInfo.ListItems(j)
                Set lstX2 = lvwInfo.ListItems(j + 1)
                lstX.Text = lstX2.Text - 1
                lstX.SubItems(1) = lstX2.SubItems(1)
                lstX.SubItems(2) = lstX2.SubItems(2)
                lstX.SubItems(3) = lstX2.SubItems(3)
                lstX.SubItems(4) = lstX2.SubItems(4)
                lstX.SubItems(5) = lstX2.SubItems(5)
                lstX.SubItems(6) = lstX2.SubItems(6)
                lstX.SubItems(7) = lstX2.SubItems(7)
            Next j
            lvwInfo.ListItems.Remove lvwInfo.ListItems.Count
            Exit For
        End If
    Next i
errHandle:
End Sub



Private Function SearchSearchById(id As Long, search As udtSearch) As Long
    On Error GoTo errHandle
    Dim i As Long
    For i = 0 To UBound(searchs)
        If searchs(i).search_id = id Then
            search = searchs(i)
            SearchSearchById = i
            Exit Function
        End If
    Next i
    
errHandle:
    SearchSearchById = -1
End Function



Private Function SearchInfoById(id As Long, info As udtInfo) As Boolean
    On Error GoTo errHandle
    Dim i As Long
    For i = 0 To UBound(infos)
        If infos(i).id = id Then
            info = infos(i)
            SearchInfoById = True
            Exit Function
        End If
    Next i
    
errHandle:
End Function

Private Function SearchInfosById(id As Long, retinfos() As udtInfo) As Boolean
    On Error GoTo errHandle
    Dim i As Long, cnt As Long
    cnt = 0
    For i = 0 To UBound(infos)
        If infos(i).search_id = id Then
            ReDim Preserve retinfos(cnt)
            retinfos(cnt) = infos(i)
            SearchInfosById = True
            cnt = cnt + 1
        End If
    Next i
    
errHandle:
End Function


'////////////////////////////////////////////////////////
'
'               private  function
'
'////////////////////////////////////////////////////////

Private Function CRC(bytall() As Byte, lngCRC As Long) As Boolean
    Dim i As Long

    Dim byt1 As Byte, byt2 As Byte, byt3 As Byte, byt4 As Byte
    byt1 = 0: byt2 = 0: byt3 = 0: byt4 = 0
    For i = 0 To UBound(bytall) Step 4
        byt1 = byt1 Xor bytall(i)
        byt2 = byt2 Xor bytall(i + 1)
        byt3 = byt3 Xor bytall(i + 2)
        byt4 = byt4 Xor bytall(i + 3)
    Next i
    If byt1 = 0 And byt2 = 0 And byt3 = 0 And byt4 = 0 Then
        CRC = True
    Else
        CRC = False
    End If
    lngCRC = GetLong(bytall, 12)
End Function

Private Sub CRCFile(file As String)
    Dim byt() As Byte
    Dim byts(3) As Byte
    Dim intFile As Integer, dword As Long
    intFile = FreeFile
    Open file For Binary As #intFile
        ReDim byt(((LOF(intFile) + 3) \ 4) * 4 - 1)
        Get intFile, , byt
        Dim i As Long, j As Long
        byts(0) = 0: byts(1) = 0: byts(2) = 0: byts(3) = 0
        For i = 0 To UBound(byt) Step 4
            For j = 0 To 3
                byts(j) = byts(j) Xor byt(i + j)
            Next j
        Next i
        dword = GetLong(byts, 0): Put intFile, 13, dword
    Close intFile
End Sub


Private Function GetLong(arr() As Byte, start As Long) As Long
    CopyMemory GetLong, arr(start), 4
    'GetLong = arr(0 + start) + arr(1 + start) * &H100& + arr(2 + start) * &H10000 + arr(3 + start) * &H1000000
    start = start + 4
End Function

'Private Sub SetLong(arr() As Byte, ByVal lng As Long)
'    ReDim arr(3)
'    arr(0) = lng Mod &H100: lng = lng \ &H100
'    arr(1) = lng Mod &H100: lng = lng \ &H100
'    arr(2) = lng Mod &H100: arr(3) = lng \ &H100
'End Sub

Private Function GetString(arr() As Byte, start As Long, len_ As Long, Optional bunicode = False) As String
    Dim i As Long, j As Long
    Dim tmparr() As Byte
    Dim key As String, key_len As Long
    key = StrConv("rsextlib", vbFromUnicode): key_len = LenB(key)
    If len_ = 0 Then GetString = "": Exit Function
    ReDim tmparr(len_ - 1)
    Call ArrCpy(tmparr, arr, start, len_)
    j = 1
    For i = 0 To len_ - 1
        If j > key_len Then j = 1
        tmparr(i) = tmparr(i) Xor AscB(MidB(key, j, 1))
        j = j + 1
    Next i
    If bunicode Then
        GetString = StrConv(tmparr, vbUnicode)
    Else
        GetString = tmparr
    End If
    start = start + len_
End Function

Private Function SetString(arr() As Byte, str As String, Optional bunicode = False)
    On Error GoTo errHandle
    Dim key As String, key_len As Long
    key = StrConv("rsextlib", vbFromUnicode): key_len = LenB(key)
    Dim i As Long, j As Long
    If bunicode Then
        arr = StrConv(str, vbFromUnicode)
    Else
        arr = str
    End If
    j = 1
    For i = 0 To UBound(arr)
        If j > key_len Then j = 1
        arr(i) = arr(i) Xor AscB(MidB(key, j, 1))
        j = j + 1
    Next i
errHandle:
End Function

Private Function HexString(str As String) As String
    Dim i As Long
    For i = 1 To LenB(str)
         HexString = HexString & Right("00" & Hex(AscB(MidB(str, i, 1))), 2)
    Next i
End Function

Private Function StringHex(str As String) As String
    Dim arr() As Byte, i As Long
    If str = "" Then StringHex = "": Exit Function
    ReDim arr((Len(str) + 1) \ 2 - 1)
    For i = 1 To Len(str) Step 2
        arr(i \ 2) = ByteHex(Mid(str, i, 2))
    Next i
    StringHex = arr
End Function

Private Function ByteHex(str As String) As Byte
    If Len(str) > 2 Then ByteHex = 0: Exit Function
    If Len(str) = 0 Then ByteHex = 0: Exit Function
    If Len(str) = 1 Then ByteHex = AsciiHex(str): Exit Function
    ByteHex = AsciiHex(Mid(str, 1, 1)) * &H10 + AsciiHex(Mid(str, 2, 1))
End Function
Private Function AsciiHex(str As String) As Byte
    If str = "" Then AsciiHex = 0: Exit Function
    If "A" <= UCase(Mid(str, 1, 1)) And UCase(Mid(str, 1, 1)) <= "F" Then
        AsciiHex = Asc(UCase(Mid(str, 1, 1))) - Asc("A") + 10: Exit Function
    ElseIf "0" <= UCase(Mid(str, 1, 1)) And UCase(Mid(str, 1, 1)) <= "9" Then
        AsciiHex = Asc(UCase(Mid(str, 1, 1))) - Asc("0"): Exit Function
    Else
        AsciiHex = 0
    End If
End Function

Private Sub ArrCpy(arr1() As Byte, arr2() As Byte, arr2start As Long, cnt As Long)
    'Dim i As Long
    'For i = 0 To cnt - 1
    '    arr1(i) = arr2(i + arr2start)
    'Next i
    CopyMemory arr1(0), arr2(arr2start), cnt
End Sub

Private Function ArrCntSearch(arr() As udtSearch) As Long
    On Error GoTo errHandle
    ArrCntSearch = UBound(arr) + 1
    Exit Function
errHandle:
    ArrCntSearch = 0
End Function
Private Function ArrCntInfo(arr() As udtInfo) As Long
    On Error GoTo errHandle
    ArrCntInfo = UBound(arr) + 1
    Exit Function
errHandle:
    ArrCntInfo = 0
End Function

Private Sub ClearForm()
    lvwSearch.ListItems.Clear
    Call ClearFormNolvw
End Sub
Private Sub ClearFormNolvw()
    Call ClearSearchCount
    Call ClearInfo
    txtSearchDesc.Text = ""
End Sub

Private Sub ClearSearchCount()
    lvwSearchCount.ListItems.Clear
    Call ClearSearchCountNolvw
End Sub
Private Sub ClearSearchCountNolvw()
    txtSearchPos.Text = ""
    txtSearchLen.Text = ""
End Sub


Private Sub ClearInfo()
    lvwInfo.ListItems.Clear
    Call ClearInfoNolvw
End Sub
Private Sub ClearInfoNolvw()
    Call ClearVersion
    Call ClearPEString
    txtArchiveType.Text = ""
    txtCorp.Text = ""
    txtWeb.Text = ""
    txtArchiveString.Text = ""
    txtSFXPos.Text = ""
    txtIPos.Text = ""
    txtIString.Text = ""
End Sub

Private Sub ClearVersion()
    lvwVersion.ListItems.Clear
    Call ClearVersionNolvw
End Sub
Private Sub ClearVersionNolvw()
    txtVersion.Text = ""
    txtVersionDesc.Text = ""
End Sub

Private Sub ClearPEString()
    lvwPEString.ListItems.Clear
    ClearPeStringNolvw
End Sub
Private Sub ClearPeStringNolvw()
    txtPEString.Text = ""
End Sub




