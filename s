-- ⚠️ Khởi tạo thư viện Fluent GUI
local Fluent = loadstring(game:HttpGet("https://github.com/dawid-scripts/Fluent/releases/latest/download/main.lua"))()
local SaveManager = loadstring(game:HttpGet("https://raw.githubusercontent.com/dawid-scripts/Fluent/master/Addons/SaveManager.lua"))()
local InterfaceManager = loadstring(game:HttpGet("https://raw.githubusercontent.com/dawid-scripts/Fluent/master/Addons/InterfaceManager.lua"))()

local Players = game:GetService("Players")
local LocalPlayer = Players.LocalPlayer
local PlayerGui = LocalPlayer:WaitForChild("PlayerGui")

local Window = Fluent:CreateWindow({
    Title = "DucWare",
    SubTitle = "by Duck (Thanks To Fluent GUI)",
    TabWidth = 160,
    Size = UDim2.fromOffset(570, 440),
    Acrylic = true,
    Theme = "Darker",
    MinimizeKey = Enum.KeyCode.LeftControl
})

local Tabs = {
    Main = Window:AddTab({ Title = "Main", Icon = "box" }),
    Player = Window:AddTab({ Title = "Player", Icon = "swords" }),
    CodeAndFun = Window:AddTab({ Title = "CodeAndFun", Icon = "box" }),
    Settings = Window:AddTab({ Title = "Settings", Icon = "settings" })
}

Fluent:Notify({
    Title = "Chào mừng",
    Content = "AnimeSaga Script ĐÃ CHẠY",
    Duration = 6
---------------Main--------------
------------------Player------------
--------------CodeAndFun--------------
---------------- SETTINGS -----------------

InterfaceManager:SetLibrary(Fluent)
SaveManager:SetLibrary(Fluent)

SaveManager:IgnoreThemeSettings()
SaveManager:SetIgnoreIndexes({})
InterfaceManager:SetFolder("FluentScriptHub")
SaveManager:SetFolder("FluentScriptHub/AnimeSaga")

InterfaceManager:BuildInterfaceSection(Tabs.Settings)
SaveManager:BuildConfigSection(Tabs.Settings)

Window:SelectTab(1)

Fluent:Notify({
    Title = "✅ DucWare đã chạy",
    Content = "Mọi chức năng đã load xong!",
    Duration = 6
})

SaveManager:LoadAutoloadConfig()
