' -------------------------------------------------------------------------------  
' Script Created by RMxprt Version 2016.0 to generate netlist for external circuit 
' Expect 3 args, first the project name,                                           
'                second the design name,                                           
'                third the path to export netlist                                  
' ---------------------------------------------------------------------------------
Dim oAnsoftApp
Dim oDesktop
Dim oProject
Dim oDesign
Dim oEditor
Dim oModule
On Error Resume Next
Set oAnsoftApp = CreateObject("Ansoft.ElectronicsDesktop")
On Error Goto 0
Set oArgs = AnsoftScript.arguments
Set oDesktop = oAnsoftApp.GetAppDesktop()
Set oProject = oDesktop.GetActiveProject
if (oArgs.Count > 0) then 
  oProject.InsertDesign "Maxwell Circuit", oArgs(0), "", ""
else
  oProject.InsertDesign "Maxwell Circuit", "", "", ""
end if
Set oDesign = oProject.GetActiveDesign()
Set oEditor = oDesign.SetActiveEditor("SchematicEditor")
oDesign.ChangeProperty Array("NAME:AllTabs", Array("NAME:LocalVariableTab", _
  Array("NAME:PropServers", "LocalVariables"), Array("NAME:NewProps", Array(_
  "NAME:Kle", "PropType:=", "VariableProp", "UserDef:=", true, "Value:=", "1"))))
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", "ECE3_Model", _
  "Id:=", "14"), Array("NAME:Attributes", "Page:=", 1, "X:=", 0, "Y:=", 0, _
  "Degrees:=", 0, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECE3_Model;14;0"), Array(_
  "NAME:ChangedProps", Array("NAME:Windings", "OverridingDef:=", true, _
  "Value:=", "PhaseA, PhaseB, PhaseC"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECE3_Model;14;0"), Array(_
  "NAME:ChangedProps", Array("NAME:CurrentSweeps", "OverridingDef:=", true, _
  "Value:=", "(5A, 2)"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECE3_Model;14;0"), Array(_
  "NAME:ChangedProps", Array("NAME:Res", "OverridingDef:=", true, "Value:=", _
  "11.9575ohm"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECE3_Model;14;0"), Array(_
  "NAME:ChangedProps", Array("NAME:IndE", "OverridingDef:=", true, "Value:=", _
  "0.00860421H*Kle"))))
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", "ECER_Model", _
  "Id:=", "21"), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.0127, "Y:=", _
  0, "Degrees:=", 0, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECER_Model;21;0"), Array(_
  "NAME:ChangedProps", Array("NAME:RotAngMax", "OverridingDef:=", true, _
  "Value:=", "60"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECER_Model;21;0"), Array(_
  "NAME:ChangedProps", Array("NAME:RotAngIntervals", "OverridingDef:=", _
  true, "Value:=", "0"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECER_Model;21;0"), Array(_
  "NAME:ChangedProps", Array("NAME:SkewAng", "OverridingDef:=", true, _
  "Value:=", "0deg"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECER_Model;21;0"), Array(_
  "NAME:ChangedProps", Array("NAME:Poles", "OverridingDef:=", true, "Value:=", _
  "4"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECER_Model;21;0"), Array(_
  "NAME:ChangedProps", Array("NAME:Slots", "OverridingDef:=", true, "Value:=", _
  "0"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@ECER_Model;21;0"), Array(_
  "NAME:ChangedProps", Array("NAME:Output", "OverridingDef:=", true, _
  "Value:=", "2"))))
oEditor.CreateGround Array("NAME:GroundProps", "Id:=", "7"), Array(_
  "NAME:Attributes", "Page:=", 1, "X:=", 0.0254, "Y:=", 0, "Degrees:=", 0, _
  "Flip:=", false) 
if (oArgs.Count > 0) then 
  oProject.SetActiveDesign oArgs(0)
end if
oEditor.ZoomToFit
if (oArgs.Count > 1) then 
  oDesign.ExportNetList "", oArgs(1)
end if
