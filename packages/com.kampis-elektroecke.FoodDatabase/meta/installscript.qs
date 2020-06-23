function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/Food Database.exe", "@StartMenuDir@/Food Database.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icons8-food-96.ico", "description=Food Database");
    }
}