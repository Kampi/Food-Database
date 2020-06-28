function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if(systemInfo.productType === "windows")
	{
        component.addOperation("CreateShortcut", "@TargetDir@/FoodDatabase.exe", "@StartMenuDir@/FoodDatabase.lnk", "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icons8-food-96.ico", "description=Food Database");
        component.addOperation("CreateShortcut", "@TargetDir@/FoodDatabase.exe", "@DesktopDir@/FoodDatabase.lnk", "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icons8-food-96.ico", "description=Food Database");
    }
}