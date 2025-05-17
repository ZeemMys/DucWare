--⚠️
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
    --Player = Window:AddTab({ Title = "Player", Icon = "swords" }),
    --CodeAndFun = Window:AddTab({ Title = "CodeAndFun", Icon = "box" }),
    Settings = Window:AddTab({ Title = "Settings", Icon = "settings" })
}

Fluent:Notify({
    Title = "Chào mừng",
    Content = "AnimeSaga Script ĐÃ CHẠY",
    Duration = 6,
})

---------------Main--------------
Tabs.Main:CreateToggle({
    Name = "Auto Harvest",
    CurrentValue = false,
    Callback = function(Value)
        _G.AutoHarvestUniversal = Value

        if not Value then
            warn("⛔ Đã tắt Auto Harvest")
            return
        end

        task.spawn(function()
            local Players = game:GetService("Players")
            local player = Players.LocalPlayer
            local hrp = player.Character:WaitForChild("HumanoidRootPart")

            -- 🌀 Teleport đến part (cao hơn 4 stud để không mắc kẹt)
            local function teleportTo(part)
                if not part or not part:IsA("BasePart") then return end
                local targetPosition = part.Position + Vector3.new(0, 4, 0)
                hrp.CFrame = CFrame.new(targetPosition)
                task.wait(0.5)
            end

            -- 🎯 Hái một prompt nếu khả thi
            local function tryHarvestPrompt(prompt, target)
                if not prompt or not prompt:IsA("ProximityPrompt") then return end
                if not target or not target:IsA("BasePart") then return end

                teleportTo(target)

                local startTime = tick()
                while tick() - startTime < 3 do
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

            -- ✅ Tìm farm của người chơi thông qua SurfaceGui.TextLabel
            local function getMyFarm()
                local farmFolder = workspace:FindFirstChild("Farm")
                if not farmFolder then return nil end

                local myNameLower = player.Name:lower()

                for _, farm in ipairs(farmFolder:GetChildren()) do
                    local sign = farm:FindFirstChild("Sign")
                    local core = sign and sign:FindFirstChild("Core_Part")
                    if core then
                        for _, obj in ipairs(core:GetDescendants()) do
                            if obj:IsA("TextLabel") and obj.Text then
                                local text = obj.Text
                                local ownerName =
                                    text:match("^(.-)'s Garden") or
                                    text:match("^(.-)’s Garden") -- hỗ trợ cả dấu nháy Unicode
                                if ownerName and ownerName:lower() == myNameLower then
                                    print("✅ Tìm đúng farm của bạn:", ownerName)
                                    return farm
                                end
                            end
                        end
                    end
                end

                warn("❌ Không tìm được farm thuộc về bạn.")
                return nil
            end

            -- 🌱 Kiểm tra cây có quả chưa
            local function isPlantMature(plant)
                local fruitsFolder = plant:FindFirstChild("Fruits")
                if fruitsFolder then
                    local growFolder = fruitsFolder:FindFirstChild("Grow") or fruitsFolder
                    for _, fruit in ipairs(growFolder:GetChildren()) do
                        if fruit:IsA("BasePart") or fruit:IsA("MeshPart") then
                            return true
                        end
                    end
                end
                return false
            end

            -- 🔁 Vòng lặp chính
            while task.wait(1) do
                if not _G.AutoHarvestUniversal then break end

                pcall(function()
                    local farm = getMyFarm()
                    if not farm then return end

                    local important = farm:FindFirstChild("Important")
                    local plantsFolder = important and important:FindFirstChild("Plants_Physical")
                    if not plantsFolder then return end

                    for _, plant in ipairs(plantsFolder:GetChildren()) do
                        if not _G.AutoHarvestUniversal then return end

                        local harvested = false

                        -- 🌳 Cây có Fruits
                        local fruitsFolder = plant:FindFirstChild("Fruits")
                        if fruitsFolder and isPlantMature(plant) then
                            local growFolder = fruitsFolder:FindFirstChild("Grow") or fruitsFolder
                            for _, fruit in ipairs(growFolder:GetChildren()) do
                                if not _G.AutoHarvestUniversal then return end
                                if fruit:IsA("BasePart") or fruit:IsA("MeshPart") then
                                    local prompt = fruit:FindFirstChildWhichIsA("ProximityPrompt")
                                    if prompt then
                                        tryHarvestPrompt(prompt, fruit)
                                        harvested = true
                                    end
                                end
                            end
                        end

                        -- 🥕 Cây không có Fruits → hái trực tiếp từ part
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
    end,
})

Tabs.Main:CreateToggle({
    Name = " Auto Sell",
    CurrentValue = false,
    Callback = function(Value)
        _G.AutoInstantSell = Value

        if Value then
            task.spawn(function()
                local player = game.Players.LocalPlayer
                local character = player.Character or player.CharacterAdded:Wait()
                local hrp = character:WaitForChild("HumanoidRootPart")
                local sellRemote = game:GetService("ReplicatedStorage").GameEvents:FindFirstChild("Sell_Inventory")
                local sellPoint = workspace:WaitForChild("Tutorial_Points"):WaitForChild("Tutorial_Point_2")

                while _G.AutoInstantSell do
                    pcall(function()
                        -- 🌀 Dịch chuyển đến chỗ bán
                        hrp.CFrame = sellPoint.CFrame + Vector3.new(0, 3, 0)
                        task.wait(0.4)

                        -- 🛒 Gửi lệnh bán
                        if sellRemote then
                            sellRemote:FireServer()
                            print("✅ Đã bán hàng (bằng teleport).")
                        else
                            warn("❌ Không tìm thấy remote Sell_Inventory")
                        end
                    end)

                    task.wait(15) -- ⏳ Đợi 15 giây trước khi bán tiếp
                end
            end)
        else
            print("⛔ Auto Sell đã bị tắt.")
        end
    end
})

Tabs.Main:CreateButton({
    Name = " Mở Seed GUI",
    Callback = function()
        local player = game.Players.LocalPlayer
        local hrp = player.Character and player.Character:FindFirstChild("HumanoidRootPart")

        pcall(function()
            hrp = player.Character and player.Character:FindFirstChild("HumanoidRootPart")
            local tpPoint = workspace:FindFirstChild("Tutorial_Points") and workspace.Tutorial_Points:FindFirstChild("Tutorial_Point_1")
            local prompt = workspace:FindFirstChild("NPCS")
                and workspace.NPCS:FindFirstChild("Sam")
                and workspace.NPCS.Sam:FindFirstChild("HumanoidRootPart")
                and workspace.NPCS.Sam.HumanoidRootPart:FindFirstChildOfClass("ProximityPrompt")

            if hrp and tpPoint and prompt then
                -- 📍 Teleport đến NPC
                hrp.CFrame = tpPoint.CFrame + Vector3.new(0, 3, 0)
                print("📍 Đã dịch chuyển tới Tutorial_Point_1")

                task.wait(0.5)

                -- ✅ Kích hoạt prompt để mở GUI
                fireproximityprompt(prompt)
                print("✅ Đã mở bảng Seed GUI")
            else
                warn("⚠️ Không tìm thấy vị trí hoặc prompt.")
            end
        end)
    end
})

Tabs.Main:CreateButton({
    Name = "Gear GUI", -- Tên hiển thị
    Callback = function()
        local player = game.Players.LocalPlayer
        local hrp = player.Character and player.Character:FindFirstChild("HumanoidRootPart")

        -- Bọc bằng pcall để tránh crash nếu có lỗi
        pcall(function()
            hrp = player.Character and player.Character:FindFirstChild("HumanoidRootPart")
            local tpPoint = workspace:FindFirstChild("Tutorial_Points") and workspace.Tutorial_Points:FindFirstChild("Tutorial_Point_3")
            local prompt = workspace:FindFirstChild("NPCS")
                and workspace.NPCS:FindFirstChild("Eloise")
                and workspace.NPCS.Eloise:FindFirstChild("HumanoidRootPart")
                and workspace.NPCS.Eloise.HumanoidRootPart:FindFirstChildOfClass("ProximityPrompt")

            local buyGearEvent = game:GetService("ReplicatedStorage"):WaitForChild("GameEvents"):FindFirstChild("BuyGearStock")

            if hrp and tpPoint and prompt then
                -- 📍 Dịch chuyển tới điểm gần NPC
                hrp.CFrame = tpPoint.CFrame + Vector3.new(0, 3, 0)
                print("📍 Đã dịch chuyển tới Tutorial_Point_3")

                task.wait(0.5)

                -- ✅ Tương tác mở GUI
                fireproximityprompt(prompt)
                print("✅ Đã mở Gear Shop")

                -- 🛒 Gửi sự kiện mua gear
                if buyGearEvent then
                    buyGearEvent:FireServer()
                    print("✅ Đã gọi sự kiện BuyGearStock")
                else
                    warn("⚠️ Không tìm thấy sự kiện BuyGearStock.")
                end
            else
                warn("⚠️ Không tìm thấy vị trí hoặc prompt.")
            end
        end)
    end
})

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
