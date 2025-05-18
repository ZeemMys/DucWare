--⚠️ Load thư viện Fluent UI
local Fluent = loadstring(game:HttpGet("https://github.com/dawid-scripts/Fluent/releases/latest/download/main.lua"))()
local SaveManager = loadstring(game:HttpGet("https://raw.githubusercontent.com/dawid-scripts/Fluent/master/Addons/SaveManager.lua"))()
local InterfaceManager = loadstring(game:HttpGet("https://raw.githubusercontent.com/dawid-scripts/Fluent/master/Addons/InterfaceManager.lua"))()

local Players = game:GetService("Players")

--⚙️ Tạo cửa sổ chính
local Window = Fluent:CreateWindow({
    Title = "DucWare",
    SubTitle = "by Duck (Thanks To Fluent GUI)",
    TabWidth = 160,
    Size = UDim2.fromOffset(570, 440),
    Acrylic = true,
    Theme = "Darker",
    MinimizeKey = Enum.KeyCode.LeftControl
})

--📁 Tabs
local Tabs = {
    Game = Window:AddTab({ Title = "Game", Icon = "sword" }),
    Player = Window:AddTab({ Title = "Player", Icon = "box" }),
    EspFruit = Window:AddTab({ Title = "Esp Enchant Fruit", Icon = "box"}),
    Settings = Window:AddTab({ Title = "Settings", Icon = "settings"}),
    Other = Window:AddTab({ Title = "Other", Icon = "settings"})
}

-- Phát hiện executor
local executor = identifyexecutor and identifyexecutor() or "Không xác định"

-- Thêm nhãn executor vào GUI
Tabs.Game:AddParagraph({
    Title = "Executor đang dùng:",
    Content = executor
})

--🔔 Thông báo khởi động
Fluent:Notify({
    Title = "Chào mừng",
    Content = "Grow a Garden Script ĐÃ CHẠY",
    Duration = 6,
})
--------------Game---------------
-- 🌾 Auto Harvest
Tabs.Game:AddToggle("auto_harvest", {
    Title = "🌾 Auto Harvest",
    Default = false,
    Callback = function(Value)
        _G.AutoHarvestUniversal = Value
        if not Value then return end

        task.spawn(function()
            local player = Players.LocalPlayer
            local hrp = player.Character:WaitForChild("HumanoidRootPart")

            local function teleportTo(part)
                if part and part:IsA("BasePart") then
                    hrp.CFrame = CFrame.new(part.Position + Vector3.new(0, 4, 0))
                    task.wait(0.5)
                end
            end

            local function tryHarvestPrompt(prompt, target)
                teleportTo(target)
                local start = tick()
                while tick() - start < 3 do
                    if not _G.AutoHarvestUniversal then return end
                    if prompt.Enabled and (target.Position - hrp.Position).Magnitude <= prompt.MaxActivationDistance then
                        fireproximityprompt(prompt)
                        task.wait(0.5)
                        return true
                    end
                    task.wait(0.1)
                end
                return false
            end

            local function getMyFarm()
                local farmFolder = workspace:FindFirstChild("Farm")
                if not farmFolder then return nil end
                local myName = player.Name:lower()

                for _, farm in ipairs(farmFolder:GetChildren()) do
                    local sign = farm:FindFirstChild("Sign")
                    local core = sign and sign:FindFirstChild("Core_Part")
                    if core then
                        for _, obj in ipairs(core:GetDescendants()) do
                            if obj:IsA("TextLabel") then
                                local name = obj.Text:match("^(.-)'s Garden") or obj.Text:match("^(.-)’s Garden")
                                if name and name:lower() == myName then
                                    return farm
                                end
                            end
                        end
                    end
                end
                return nil
            end

            local function isPlantMature(plant)
                local fruits = plant:FindFirstChild("Fruits")
                local grow = fruits and (fruits:FindFirstChild("Grow") or fruits)
                if grow then
                    for _, fruit in ipairs(grow:GetChildren()) do
                        if fruit:IsA("BasePart") or fruit:IsA("MeshPart") then
                            return true
                        end
                    end
                end
                return false
            end

            while task.wait(1) do
                if not _G.AutoHarvestUniversal then break end

                pcall(function()
                    local farm = getMyFarm()
                    if not farm then return end
                    local plants = farm:FindFirstChild("Important") and farm.Important:FindFirstChild("Plants_Physical")
                    if not plants then return end

                    for _, plant in ipairs(plants:GetChildren()) do
                        if not _G.AutoHarvestUniversal then return end

                        local harvested = false
                        local fruits = plant:FindFirstChild("Fruits")
                        if fruits and isPlantMature(plant) then
                            local grow = fruits:FindFirstChild("Grow") or fruits
                            for _, fruit in ipairs(grow:GetChildren()) do
                                if not _G.AutoHarvestUniversal then return end
                                local prompt = fruit:FindFirstChildWhichIsA("ProximityPrompt")
                                if prompt then
                                    tryHarvestPrompt(prompt, fruit)
                                    harvested = true
                                end
                            end
                        end

                        if not harvested then
                            for _, part in ipairs(plant:GetDescendants()) do
                                if not _G.AutoHarvestUniversal then return end
                                if part:IsA("BasePart") then
                                    local prompt = part:FindFirstChildWhichIsA("ProximityPrompt")
                                    if prompt then
                                        tryHarvestPrompt(prompt, part)
                                    end
                                end
                            end
                        end
                    end
                end)
            end
        end)
    end
})

-- 💰 Auto Sell
Tabs.Game:AddToggle("auto_sell", {
    Title = "💰 Auto Sell",
    Default = false,
    Callback = function(Value)
        _G.AutoInstantSell = Value

        if Value then
            task.spawn(function()
                local player = Players.LocalPlayer
                local hrp = player.Character:WaitForChild("HumanoidRootPart")
                local sellRemote = game:GetService("ReplicatedStorage").GameEvents:FindFirstChild("Sell_Inventory")
                local sellPoint = workspace:WaitForChild("Tutorial_Points"):WaitForChild("Tutorial_Point_2")

                while _G.AutoInstantSell do
                    pcall(function()
                        hrp.CFrame = sellPoint.CFrame + Vector3.new(0, 3, 0)
                        task.wait(0.4)
                        if sellRemote then
                            sellRemote:FireServer()
                        end
                    end)
                    task.wait(15)
                end
            end)
        end
    end
})
---------------------Player-----------------
local Players = game:GetService("Players")
local LocalPlayer = Players.LocalPlayer

-- WalkSpeed
local WalkEnabled = false
local DefaultWalkSpeed = 16
local WalkSpeedValue = DefaultWalkSpeed

Tabs.Player:AddToggle("walkspeed", {
    Title = "🏃 Chạy Nhanh",
    Default = false,
    Callback = function(Value)
        WalkEnabled = Value
        local humanoid = LocalPlayer.Character and LocalPlayer.Character:FindFirstChildOfClass("Humanoid")
        if humanoid then
            humanoid.WalkSpeed = Value and WalkSpeedValue or DefaultWalkSpeed
        end
    end,
})

Tabs.Player:AddSlider("walkspeed_slider", {
    Title = "Tốc độ chạy",
    Description = "Điều chỉnh tốc độ",
    Default = DefaultWalkSpeed,
    Min = 16,
    Max = 200,
    Rounding = 0,
    Callback = function(Value)
        WalkSpeedValue = Value
        if WalkEnabled then
            local humanoid = LocalPlayer.Character and LocalPlayer.Character:FindFirstChildOfClass("Humanoid")
            if humanoid then
                humanoid.WalkSpeed = Value
            end
        end
    end,
})

-- JumpPower
local JumpEnabled = false
local DefaultJumpPower = 50
local JumpPowerValue = DefaultJumpPower

Tabs.Player:AddToggle("jumppower", {
    Title = "🦘 Sức Mạnh Nhảy",
    Default = false,
    Callback = function(Value)
        JumpEnabled = Value
        local humanoid = LocalPlayer.Character and LocalPlayer.Character:FindFirstChildOfClass("Humanoid")
        if humanoid then
            humanoid.JumpPower = Value and JumpPowerValue or DefaultJumpPower
        end
    end,
})

Tabs.Player:AddSlider("jumppower_slider", {
    Title = "Độ cao nhảy",
    Default = DefaultJumpPower,
    Min = 50,
    Max = 500,
    Rounding = 0,
    Callback = function(Value)
        JumpPowerValue = Value
        if JumpEnabled then
            local humanoid = LocalPlayer.Character and LocalPlayer.Character:FindFirstChildOfClass("Humanoid")
            if humanoid then
                humanoid.JumpPower = Value
            end
        end
    end,
})

-- Aimbot
local AimbotEnabled = false
local AimbotFOV = 100
local currentTarget = nil
local marker = nil

local function createMarker(targetPart)
    if marker then marker:Destroy() end
    marker = Instance.new("BillboardGui")
    marker.Name = "AimbotMarker"
    marker.AlwaysOnTop = true
    marker.Size = UDim2.new(0, 12, 0, 12)
    marker.Adornee = targetPart
    marker.Parent = targetPart

    local dot = Instance.new("Frame")
    dot.Size = UDim2.new(1, 0, 1, 0)
    dot.BackgroundColor3 = Color3.fromRGB(0, 255, 0)
    dot.BorderSizePixel = 0
    dot.BackgroundTransparency = 0.1
    dot.Parent = marker
end

local function removeMarker()
    if marker then
        marker:Destroy()
        marker = nil
    end
end

local function GetNearestTarget()
    local camera = workspace.CurrentCamera
    local nearest, shortestDistance = nil, AimbotFOV
    for _, player in ipairs(Players:GetPlayers()) do
        if player ~= LocalPlayer and player.Character and player.Character:FindFirstChild("HumanoidRootPart") then
            local pos, onScreen = camera:WorldToViewportPoint(player.Character.HumanoidRootPart.Position)
            if onScreen then
                local distance = (Vector2.new(pos.X, pos.Y) - camera.ViewportSize / 2).Magnitude
                if distance < shortestDistance then
                    shortestDistance = distance
                    nearest = player
                end
            end
        end
    end
    return nearest
end

local aimbotConnection
local function enableAimbot()
    aimbotConnection = game:GetService("RunService").RenderStepped:Connect(function()
        if not AimbotEnabled then return end
        local target = GetNearestTarget()
        local cam = workspace.CurrentCamera

        if target and target.Character and target.Character:FindFirstChild("HumanoidRootPart") then
            local targetPos = target.Character.HumanoidRootPart.Position
            local camPos = cam.CFrame.Position
            cam.CFrame = CFrame.new(camPos, targetPos)

            if target ~= currentTarget then
                currentTarget = target
                createMarker(target.Character.HumanoidRootPart)
            end
        else
            if currentTarget then
                currentTarget = nil
                removeMarker()
            end
        end
    end)
end

local function disableAimbot()
    if aimbotConnection then aimbotConnection:Disconnect() end
    currentTarget = nil
    removeMarker()
    workspace.CurrentCamera.CameraType = Enum.CameraType.Custom
end

Tabs.Player:AddToggle("aimbot_toggle", {
    Title = "🎯 Auto Aimbot",
    Default = false,
    Callback = function(Value)
        AimbotEnabled = Value
        if Value then
            enableAimbot()
        else
            disableAimbot()
        end
    end,
})

Tabs.Player:AddSlider("aimbot_fov", {
    Title = "Aimbot FOV",
    Min = 20,
    Max = 500,
    Default = AimbotFOV,
    Rounding = 0,
    Suffix = "px",
    Callback = function(Value)
        AimbotFOV = Value
    end,
})

local UIS = game:GetService("UserInputService")
local RunService = game:GetService("RunService")
local player = game.Players.LocalPlayer
local character = player.Character or player.CharacterAdded:Wait()
local root = character:WaitForChild("HumanoidRootPart")

local flying = false
local flySpeed = 50 -- tốc độ bay mặc định
local bv, bg
local flyConnection

local function startFly()
    if bv then bv:Destroy() end
    if bg then bg:Destroy() end

    bv = Instance.new("BodyVelocity")
    bv.Velocity = Vector3.zero
    bv.MaxForce = Vector3.new(1, 1, 1) * 1e5
    bv.P = 1250
    bv.Parent = root

    bg = Instance.new("BodyGyro")
    bg.CFrame = root.CFrame
    bg.MaxTorque = Vector3.new(1, 1, 1) * 1e5
    bg.P = 1250
    bg.Parent = root

    flyConnection = RunService.RenderStepped:Connect(function()
        if not flying then return end

        local moveDir = Vector3.zero
        if UIS:IsKeyDown(Enum.KeyCode.W) then moveDir += workspace.CurrentCamera.CFrame.LookVector end
        if UIS:IsKeyDown(Enum.KeyCode.S) then moveDir -= workspace.CurrentCamera.CFrame.LookVector end
        if UIS:IsKeyDown(Enum.KeyCode.A) then moveDir -= workspace.CurrentCamera.CFrame.RightVector end
        if UIS:IsKeyDown(Enum.KeyCode.D) then moveDir += workspace.CurrentCamera.CFrame.RightVector end
        if UIS:IsKeyDown(Enum.KeyCode.Space) then moveDir += Vector3.new(0, 1, 0) end
        if UIS:IsKeyDown(Enum.KeyCode.LeftControl) then moveDir -= Vector3.new(0, 1, 0) end

        if moveDir.Magnitude > 0 then
            bv.Velocity = moveDir.Unit * flySpeed
        else
            bv.Velocity = Vector3.zero
        end

        bg.CFrame = workspace.CurrentCamera.CFrame
    end)
end

local function stopFly()
    flying = false
    if flyConnection then
        flyConnection:Disconnect()
        flyConnection = nil
    end
    if bv then bv:Destroy() bv = nil end
    if bg then bg:Destroy() bg = nil end
end

-- ✅ Toggle bay
Tabs.Player:AddToggle("fly_toggle", {
    Title = "🕊️ Bay",
    Default = false,
    Callback = function(state)
        flying = state
        character = player.Character or player.CharacterAdded:Wait()
        root = character:WaitForChild("HumanoidRootPart")

        if flying then
            startFly()
        else
            stopFly()
        end
    end
})

-- ✅ Slider tốc độ bay
Tabs.Player:AddSlider("fly_speed_slider", {
    Title = "Tốc độ bay",
    Min = 10,
    Max = 200,
    Default = 50,
    Rounding = 0,
    Callback = function(value)
        flySpeed = value
    end
})

local ESPEnabled = false
local ESPObjects = {}

function createESP(player)
    if player == game.Players.LocalPlayer then return end
    local character = player.Character
    if not character then return end

    -- Highlight toàn bộ nhân vật
    local highlight = Instance.new("Highlight")
    highlight.Name = "ESPHighlight"
    highlight.FillColor = Color3.fromRGB(0, 255, 0)
    highlight.FillTransparency = 0.5
    highlight.OutlineColor = Color3.new(1, 1, 1)
    highlight.OutlineTransparency = 0
    highlight.Adornee = character
    highlight.Parent = character

    -- Tên người chơi
    local nameTag = Instance.new("BillboardGui")
    nameTag.Name = "ESPName"
    nameTag.Size = UDim2.new(0, 100, 0, 40)
    nameTag.StudsOffset = Vector3.new(0, 3, 0)
    nameTag.Adornee = character:FindFirstChild("Head")
    nameTag.AlwaysOnTop = true
    nameTag.Parent = character

    local nameLabel = Instance.new("TextLabel")
    nameLabel.Size = UDim2.new(1, 0, 1, 0)
    nameLabel.BackgroundTransparency = 1
    nameLabel.Text = player.Name
    nameLabel.TextColor3 = Color3.new(1, 1, 1)
    nameLabel.TextStrokeTransparency = 0
    nameLabel.TextScaled = true
    nameLabel.Font = Enum.Font.SourceSansBold
    nameLabel.Parent = nameTag

    ESPObjects[player] = {highlight, nameTag}
end

function removeESP(player)
    if ESPObjects[player] then
        for _, v in pairs(ESPObjects[player]) do
            if v then v:Destroy() end
        end
        ESPObjects[player] = nil
    end
end

function updateESP()
    for _, player in pairs(game.Players:GetPlayers()) do
        if ESPEnabled then
            if player.Character and not ESPObjects[player] then
                createESP(player)
            end
        else
            removeESP(player)
        end
    end
end

-- Toggle ESP
Tabs.Player:AddToggle("ESP_toggle", {
    Title = "🧿 ESP Người chơi",
    Default = false,
    Callback = function(Value)
        ESPEnabled = Value
        updateESP()
    end
})

-- Theo dõi người chơi mới
game.Players.PlayerAdded:Connect(function(player)
    player.CharacterAdded:Connect(function()
        wait(1)
        if ESPEnabled then
            createESP(player)
        end
    end)
end)

-- Xoá ESP khi người chơi rời
game.Players.PlayerRemoving:Connect(function(player)
    removeESP(player)
end)
---------------------EspFruit----------------

--------------------------- ⚙️ Settings tab----------------
InterfaceManager:SetLibrary(Fluent)
SaveManager:SetLibrary(Fluent)
SaveManager:IgnoreThemeSettings()
SaveManager:SetIgnoreIndexes({})
InterfaceManager:SetFolder("FluentScriptHub")
SaveManager:SetFolder("FluentScriptHub/GrowAGarden")
InterfaceManager:BuildInterfaceSection(Tabs.Settings)
SaveManager:BuildConfigSection(Tabs.Settings)

Window:SelectTab(1)

Fluent:Notify({
    Title = "✅ DucWare đã chạy",
    Content = "Mọi chức năng đã load xong!",
    Duration = 6
})

SaveManager:LoadAutoloadConfig()
--------------Other-----------
Tabs.Other:AddButton({
    Title = "💀",
    Description = "Ấn Để Biết",
    Callback = function()
        local Players = game:GetService("Players")
        local TeleportService = game:GetService("TeleportService")
        local TweenService = game:GetService("TweenService")
        local Lighting = game:GetService("Lighting")
        local LocalPlayer = Players.LocalPlayer
        local PlaceId = game.PlaceId

        -- Blur Effect
        local blur = Instance.new("BlurEffect")
        blur.Size = 18
        blur.Name = "FakeKickBlur"
        blur.Parent = Lighting

        -- GUI
        local screen = Instance.new("ScreenGui")
        screen.Name = "FakeKickMessage"
        screen.ResetOnSpawn = false
        screen.IgnoreGuiInset = true
        screen.ZIndexBehavior = Enum.ZIndexBehavior.Sibling
        screen.Parent = LocalPlayer:WaitForChild("PlayerGui")

        -- Nền tối
        local darkBackground = Instance.new("Frame")
        darkBackground.Size = UDim2.new(1, 0, 1, 0)
        darkBackground.BackgroundColor3 = Color3.fromRGB(0, 0, 0)
        darkBackground.BackgroundTransparency = 0.5
        darkBackground.Parent = screen

        -- Khung chính
        local frame = Instance.new("Frame")
        frame.AnchorPoint = Vector2.new(0.5, 0.5)
        frame.Position = UDim2.new(0.5, 0, 0.5, 0)
        frame.Size = UDim2.new(0, 500, 0, 220)
        frame.BackgroundColor3 = Color3.fromRGB(35, 35, 35)
        frame.Parent = screen

        local corner = Instance.new("UICorner", frame)
        corner.CornerRadius = UDim.new(0, 6)

        -- Tiêu đề
        local title = Instance.new("TextLabel", frame)
        title.Text = "Disconnected"
        title.Size = UDim2.new(1, 0, 0, 40)
        title.Position = UDim2.new(0, 0, 0, 10)
        title.BackgroundTransparency = 1
        title.Font = Enum.Font.GothamSemibold
        title.TextColor3 = Color3.fromRGB(255, 255, 255)
        title.TextSize = 26

        -- Nội dung
        local content = Instance.new("TextLabel", frame)
        content.Text = "You have been kicked from this experience.\n(Error Code: 267)"
        content.Size = UDim2.new(1, -40, 0, 80)
        content.Position = UDim2.new(0, 20, 0, 50)
        content.BackgroundTransparency = 1
        content.TextColor3 = Color3.fromRGB(220, 220, 220)
        content.Font = Enum.Font.Gotham
        content.TextWrapped = true
        content.TextYAlignment = Enum.TextYAlignment.Top
        content.TextSize = 18

        -- Nút Leave
        local leave = Instance.new("TextButton", frame)
        leave.Text = "Leave"
        leave.Size = UDim2.new(0, 120, 0, 36)
        leave.Position = UDim2.new(0.5, -130, 1, -50)
        leave.BackgroundColor3 = Color3.fromRGB(90, 90, 90)
        leave.TextColor3 = Color3.new(1, 1, 1)
        leave.Font = Enum.Font.Gotham
        leave.TextSize = 18
        leave.Parent = frame

        local leaveCorner = Instance.new("UICorner", leave)
        leaveCorner.CornerRadius = UDim.new(0, 4)

        leave.MouseButton1Click:Connect(function()
            blur:Destroy()
            screen:Destroy()
            LocalPlayer:Kick("ngu vai cac")
        end)

        -- Nút Rejoin
        local rejoin = Instance.new("TextButton", frame)
        rejoin.Text = "Rejoin"
        rejoin.Size = UDim2.new(0, 120, 0, 36)
        rejoin.Position = UDim2.new(0.5, 10, 1, -50)
        rejoin.BackgroundColor3 = Color3.fromRGB(90, 90, 90)
        rejoin.TextColor3 = Color3.new(1, 1, 1)
        rejoin.Font = Enum.Font.Gotham
        rejoin.TextSize = 18
        rejoin.Parent = frame

        local rejoinCorner = Instance.new("UICorner", rejoin)
        rejoinCorner.CornerRadius = UDim.new(0, 4)

        rejoin.MouseButton1Click:Connect(function()
            blur:Destroy()
            screen:Destroy()
            TeleportService:Teleport(PlaceId, LocalPlayer)
        end)
    end
})

local TeleportService = game:GetService("TeleportService")
local Players = game:GetService("Players")
local HttpService = game:GetService("HttpService")

local LocalPlayer = Players.LocalPlayer
local PlaceId = game.PlaceId

-- NÚT 1: Join lại chính server hiện tại
Tabs.Other:AddButton({
    Title = "👤 Rejoin Server",
    Description = "Vào lại server hiện tại",
    Callback = function()
        TeleportService:Teleport(PlaceId, LocalPlayer)
    end
})

local Players = game:GetService("Players")
local RunService = game:GetService("RunService")

local LocalPlayer = Players.LocalPlayer
local tornadoActive = false
local tornadoConnection

Tabs.Other:AddToggle("TornadoToggle", {
    Title = "🌪 Tornado Mode",
    Description = "Quay vòng vòng như lốc xoáy!",
    Default = false,
    Callback = function(state)
        tornadoActive = state

        if tornadoConnection then
            tornadoConnection:Disconnect()
            tornadoConnection = nil
        end

        if state then
            tornadoConnection = RunService.RenderStepped:Connect(function()
                local char = LocalPlayer.Character
                if not char then return end
                local hrp = char:FindFirstChild("HumanoidRootPart")
                if not hrp then return end

                -- Xoay nhân vật nhẹ nhàng
                hrp.CFrame *= CFrame.Angles(0, math.rad(3), 0)
            end)
        end
    end
})
