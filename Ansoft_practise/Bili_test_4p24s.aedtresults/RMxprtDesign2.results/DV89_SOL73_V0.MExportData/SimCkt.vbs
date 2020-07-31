' -----------------------------------------------------------------------------  
' Script Created by RMxprt Version 2016.0 to generate Simplorer Circuit design   
' Expect 2 args, first the project name,                                         
'                second the design name                                          
' -------------------------------------------------------------------------------
Dim oAnsoftApp
Dim oDesktop
Dim oProject
Dim oDesign
Dim oEditor
Dim oModule
Dim oSml1, oSml2, oModName
oModName = "Setup2"

On Error Resume Next
Set oAnsoftApp = CreateObject("Ansoft.ElectronicsDesktop")
On Error Goto 0
' -----------------------------------------------------------------------------  
' Determine the path of this vbs and resolve path to other referenced files      
' -----------------------------------------------------------------------------  
Dim vbspath, dir, path, fileName, filesys, pathArray, count
vbspath = AnsoftScript.GetScriptPath
Set filesys = CreateObject("Scripting.FileSystemObject")
dir = filesys.GetParentFolderName(vbspath)
fileName = filesys.GetFileName(vbspath)
fileName = filesys.GetBaseName(fileName)
pathArray = Split(fileName, "_")
' Form the path
count = UBound(pathArray) - 1
if (count > 0) Then
For i = 0 to count
path = path + pathArray(i)
if (i < count) Then
path = path + "_"
End If
Next
oSml1 = dir + "/" + path + ".sml"
oSml2 = dir + "/" + path + "_signals.sml"
Else ' no _ in file name
oSml1 = dir + "/" + oModName + ".sml"
oSml2 = dir + "/" + "signals.sml"
End if

Set oArgs = AnsoftScript.arguments
Set oDesktop = oAnsoftApp.GetAppDesktop()
Set oProject = oDesktop.GetActiveProject()
if (oArgs.Count > 1) then 
  oProject.InsertDesign "Twin Builder", oArgs(1), "", ""
else
  oProject.InsertDesign "Twin Builder", "", "", ""
end if
Set oDesign = oProject.GetActiveDesign()
Set oEditor = oDesign.SetActiveEditor("SchematicEditor")
Set oDefinitionManager = oProject.GetDefinitionManager()
Set oComponentManager = oDefinitionManager.GetManager("Component")
oComponentManager.ImportModelsFromFile oSml1, Array("NAME:Options", "Mode:=", _
  1)
oComponentManager.ImportModelsFromFile oSml2, Array("NAME:Options", "Mode:=", _
  1)
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", "Setup2", _
  "Id:=", "35"), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.05334, "Y:=", _
  -0.03048, "Degrees:=", 0, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@Setup2;35;0"), Array(_
  "NAME:ChangedProps", Array("NAME:InstanceName", "OverridingDef:=", true, _
  "Value:=", "ASSM"))))
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "A", _
  "Id:=", 42), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04572, "Y:=", _
  -0.02794, "Degrees:=", 0, "Flip:=", false) )
oEditor.Move Array("NAME:Selections", "Selections:=", Array(ppID)), Array(_
  "NAME:MoveParameters", "xdelta:=", -0.00254, "ydelta:=", 0, "Disconnect:=", _
  false, "Rubberband:=", false)
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "B", _
  "Id:=", 49), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04572, "Y:=", _
  -0.03048, "Degrees:=", 0, "Flip:=", false) )
oEditor.Move Array("NAME:Selections", "Selections:=", Array(ppID)), Array(_
  "NAME:MoveParameters", "xdelta:=", -0.00254, "ydelta:=", 0, "Disconnect:=", _
  false, "Rubberband:=", false)
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "C", _
  "Id:=", 56), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04572, "Y:=", _
  -0.03302, "Degrees:=", 0, "Flip:=", false) )
oEditor.Move Array("NAME:Selections", "Selections:=", Array(ppID)), Array(_
  "NAME:MoveParameters", "xdelta:=", -0.00254, "ydelta:=", 0, "Disconnect:=", _
  false, "Rubberband:=", false)
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "ROT1", _
  "Id:=", 63), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.06096, "Y:=", _
  -0.02794, "Degrees:=", 180, "Flip:=", false) )
oEditor.Move Array("NAME:Selections", "Selections:=", Array(ppID)), Array(_
  "NAME:MoveParameters", "xdelta:=", 0.00254, "ydelta:=", 0, "Disconnect:=", _
  false, "Rubberband:=", false)
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "ROT2", _
  "Id:=", 70), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.06096, "Y:=", _
  -0.03048, "Degrees:=", 180, "Flip:=", false) )
oEditor.Move Array("NAME:Selections", "Selections:=", Array(ppID)), Array(_
  "NAME:MoveParameters", "xdelta:=", 0.00254, "ydelta:=", 0, "Disconnect:=", _
  false, "Rubberband:=", false)
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Physical Domains/Mechanical/Velocity-Force-Representation/Rotational_V:V_ROT", _
  "Id:=", "77"), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.09144, "Y:=", _
  -0.0254, "Degrees:=", 180, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@V_ROT;77;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "speed"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@V_ROT;77;0"), Array("NAME:ChangedProps", _
  Array("NAME:VALUE", "OverridingDef:=", true, "Value:=", "1500rpm"))))
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "ROT1", _
  "Id:=", 84), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.08636, "Y:=", _
  -0.02794, "Degrees:=", 0, "Flip:=", false) )
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "ROT2", _
  "Id:=", 91), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.08636, "Y:=", _
  -0.0381, "Degrees:=", 0, "Flip:=", false) )
oEditor.CreateGround Array("NAME:GroundProps", "Id:=", "28"), Array(_
  "NAME:Attributes", "Page:=", 1, "X:=", 0.08636, "Y:=", -0.04064, _
  "Degrees:=", 0, "Flip:=", false) 
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Measurement/Electrical:AM", "Id:=", "98"), _
  Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04318, "Y:=", 0.00762, _
  "Degrees:=", 90, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@AM;98;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "IA"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PropDisplayPropTab", _
  Array("NAME:PropServers", "CompInst@AM;98;0"), Array("NAME:NewProps", _
  Array("NAME:InstanceName", "Format:=", "Value", "Location:=", "Right"))))
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Measurement/Electrical:AM", "Id:=", _
  "105"), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04318, "Y:=", _
  -0.00254, "Degrees:=", 90, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@AM;105;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "IB"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PropDisplayPropTab", _
  Array("NAME:PropServers", "CompInst@AM;105;0"), Array("NAME:NewProps", _
  Array("NAME:InstanceName", "Format:=", "Value", "Location:=", "Right"))))
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Measurement/Electrical:AM", "Id:=", _
  "112"), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04318, "Y:=", _
  -0.0127, "Degrees:=", 90, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@AM;112;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "IC"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PropDisplayPropTab", _
  Array("NAME:PropServers", "CompInst@AM;112;0"), Array("NAME:NewProps", _
  Array("NAME:InstanceName", "Format:=", "Value", "Location:=", "Right"))))
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "A", _
  "Id:=", 119), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04826, "Y:=", _
  0.01016, "Degrees:=", 180, "Flip:=", false) )
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "B", _
  "Id:=", 126), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04826, "Y:=", _
  0, "Degrees:=", 180, "Flip:=", false) )
ppID = oEditor.CreatePagePort( Array("NAME:PagePortProps", "Name:=", "C", _
  "Id:=", 133), Array("NAME:Attributes", "Page:=", 1, "X:=", 0.04826, "Y:=", _
  -0.01016, "Degrees:=", 180, "Flip:=", false) )
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0, -0.0381)", "(0.0254, -0.0381)")), Array("NAME:Attributes", "Page:=", 1) 
oEditor.CreateGround Array("NAME:GroundProps", "Id:=", "28"), Array(_
  "NAME:Attributes", "Page:=", 1, "X:=", 0.0127, "Y:=", -0.04064, "Degrees:=", _
  0, "Flip:=", false) 
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0, -0.0381)", "(0, -0.03556)")), Array("NAME:Attributes", "Page:=", 1) 
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Circuit/Sources:E", "Id:=", "140"), _
  Array("NAME:Attributes", "Page:=", 1, "X:=", -0.00254, "Y:=", -0.03048, _
  "Degrees:=", 0, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PropDisplayPropTab", _
  Array("NAME:PropServers", "CompInst@E;140;0"), Array("NAME:NewProps", _
  Array("NAME:InstanceName", "Format:=", "Value", "Location:=", "Right"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@E;140;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "VA"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@E;140;0"), Array("NAME:ChangedProps", _
  Array("NAME:Type", "OverridingDef:=", true, "Value:=", "ESINE"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;140;0"), Array("NAME:ChangedProps", Array(_
  "NAME:AMPL", "OverridingDef:=", true, "Value:=", "179.629V"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;140;0"), Array("NAME:ChangedProps", Array(_
  "NAME:FREQ", "OverridingDef:=", true, "Value:=", "50Hz"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;140;0"), Array("NAME:ChangedProps", Array(_
  "NAME:TPERIO", "OverridingDef:=", true, "Value:=", "0.02s"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;140;0"), Array("NAME:ChangedProps", Array(_
  "NAME:PHASE", "OverridingDef:=", true, "Value:=", "38.2186deg"))))
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0, -0.0254)", "(0, 0.01016)", "(0.0381, 0.01016)")), Array(_
  "NAME:Attributes", "Page:=", 1) 
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0.0127, -0.0381)", "(0.0127, -0.03556)")), Array("NAME:Attributes", _
  "Page:=", 1) 
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Circuit/Sources:E", "Id:=", "147"), _
  Array("NAME:Attributes", "Page:=", 1, "X:=", 0.01016, "Y:=", -0.03048, _
  "Degrees:=", 0, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PropDisplayPropTab", _
  Array("NAME:PropServers", "CompInst@E;147;0"), Array("NAME:NewProps", _
  Array("NAME:InstanceName", "Format:=", "Value", "Location:=", "Right"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@E;147;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "VB"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@E;147;0"), Array("NAME:ChangedProps", _
  Array("NAME:Type", "OverridingDef:=", true, "Value:=", "ESINE"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;147;0"), Array("NAME:ChangedProps", Array(_
  "NAME:AMPL", "OverridingDef:=", true, "Value:=", "179.629V"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;147;0"), Array("NAME:ChangedProps", Array(_
  "NAME:FREQ", "OverridingDef:=", true, "Value:=", "50Hz"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;147;0"), Array("NAME:ChangedProps", Array(_
  "NAME:TPERIO", "OverridingDef:=", true, "Value:=", "0.02s"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;147;0"), Array("NAME:ChangedProps", Array(_
  "NAME:PHASE", "OverridingDef:=", true, "Value:=", "278.219deg"))))
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0.0127, -0.0254)", "(0.0127, 0)", "(0.0381, 0)")), Array(_
  "NAME:Attributes", "Page:=", 1) 
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0.0254, -0.0381)", "(0.0254, -0.03556)")), Array("NAME:Attributes", _
  "Page:=", 1) 
oEditor.CreateComponent Array("NAME:ComponentProps", "Name:=", _
  "Simplorer Elements/Basic Elements/Circuit/Sources:E", "Id:=", "154"), _
  Array("NAME:Attributes", "Page:=", 1, "X:=", 0.02286, "Y:=", -0.03048, _
  "Degrees:=", 0, "Flip:=", false) 
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PropDisplayPropTab", _
  Array("NAME:PropServers", "CompInst@E;154;0"), Array("NAME:NewProps", _
  Array("NAME:InstanceName", "Format:=", "Value", "Location:=", "Right"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@E;154;0"), Array("NAME:ChangedProps", _
  Array("NAME:InstanceName", "OverridingDef:=", true, "Value:=", "VC"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:PassedParameterTab", _
  Array("NAME:PropServers", "CompInst@E;154;0"), Array("NAME:ChangedProps", _
  Array("NAME:Type", "OverridingDef:=", true, "Value:=", "ESINE"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;154;0"), Array("NAME:ChangedProps", Array(_
  "NAME:AMPL", "OverridingDef:=", true, "Value:=", "179.629V"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;154;0"), Array("NAME:ChangedProps", Array(_
  "NAME:FREQ", "OverridingDef:=", true, "Value:=", "50Hz"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;154;0"), Array("NAME:ChangedProps", Array(_
  "NAME:TPERIO", "OverridingDef:=", true, "Value:=", "0.02s"))))
oEditor.ChangeProperty Array("NAME:AllTabs", Array("NAME:Quantities", Array(_
  "NAME:PropServers", "CompInst@E;154;0"), Array("NAME:ChangedProps", Array(_
  "NAME:PHASE", "OverridingDef:=", true, "Value:=", "158.219deg"))))
oEditor.CreateWire Array("NAME:WireData", "Id:=", "1", "Points:=", Array(_
  "(0.0254, -0.0254)", "(0.0254, -0.01016)", "(0.0381, -0.01016)")), Array(_
  "NAME:Attributes", "Page:=", 1) 
Set oModule = oDesign.GetModule("SimSetup")
oModule.EditTransient "TR", Array("NAME:SimSetup", "OptionName:=", _
  "(Default Options)", "AnalysisEnabled:=", 1, "Name:=", "TR", _
  "TransientData:=", Array("100ms", "2us", "20us", false, ""), _
  "EnableContSolve:=", false)
Set oModule = oDesign.GetModule("ReportSetup")
oModule.CreateReport "Load Torque", "Standard", "Rectangular Plot", "TR", _
  Array("NAME:Context", "SimValueContext:=", _
  Array(1, 0, 2, 0, false, false, -1, 1, 0, 1, 1, "", 0, 0)), Array("Time:=", _
  Array("All")), Array("X Component:=", "Time", "Y Component:=", Array(_
  "speed.TORQUE")), Array()
oModule.CreateReport "Current", "Standard", "Rectangular Plot", "TR", Array(_
  "NAME:Context", "SimValueContext:=", _
  Array(1, 0, 2, 0, false, false, -1, 1, 0, 1, 1, "", 0, 0)), Array("Time:=", _
  Array("All")), Array("X Component:=", "Time", "Y Component:=", Array(_
  "IA.I", "IB.I", "IC.I")), Array()
sName = oDesign.GetName()
arr = Split(sName, ";")
oProject.SetActiveDesign arr(1)
oEditor.ZoomToFit
if (oArgs.Count > 0) then 
  oProject.SaveAs oArgs(0), TRUE
end if
