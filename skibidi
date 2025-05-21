local CoreGui = game:GetService("CoreGui")
local Players = game:GetService("Players")
local UIS = game:GetService("UserInputService")
local LocalPlayer = Players.LocalPlayer

-- GUI launcher
local gui = Instance.new("ScreenGui")
gui.Name = "DucWareLauncher"
gui.ResetOnSpawn = false
gui.Parent = CoreGui

local frame = Instance.new("Frame")
frame.Size = UDim2.new(0, 100, 0, 100)
frame.Position = UDim2.new(0, 20, 0.5, -50)
frame.BackgroundColor3 = Color3.fromRGB(30, 30, 30)
frame.BorderSizePixel = 0
frame.BackgroundTransparency = 0.05
frame.Parent = gui
frame.Active = true
frame.Draggable = true
frame.AnchorPoint = Vector2.new(0, 0.5)

local corner = Instance.new("UICorner")
corner.CornerRadius = UDim.new(0, 100)
corner.Parent = frame

-- Shadow
local shadow = Instance.new("ImageLabel")
shadow.Name = "Shadow"
shadow.BackgroundTransparency = 1
shadow.Image = "rbxassetid://1316045217"
shadow.ImageColor3 = Color3.new(0, 0, 0)
shadow.ScaleType = Enum.ScaleType.Slice
shadow.SliceCenter = Rect.new(10, 10, 118, 118)
shadow.Size = UDim2.new(1, 30, 1, 30)
shadow.Position = UDim2.new(0, -15, 0, -15)
shadow.ZIndex = 0
shadow.Parent = frame

-- Logo
local logo = Instance.new("ImageButton")
logo.Size = UDim2.new(1, 0, 1, 0)
logo.Position = UDim2.new(0, 0, 0, 0)
logo.BackgroundTransparency = 1
logo.Image = "rbxassetid://10172846277981" -- Anime ngầu bạn chọn
logo.Parent = frame

-- Title
local title = Instance.new("TextLabel")
title.Size = UDim2.new(1, 0, 0, 20)
title.Position = UDim2.new(0, 0, 1, -5)
title.BackgroundTransparency = 1
title.Text = "DucWare"
title.Font = Enum.Font.GothamBold
title.TextSize = 16
title.TextColor3 = Color3.fromRGB(255, 255, 255)
title.TextStrokeTransparency = 0.7
title.TextYAlignment = Enum.TextYAlignment.Top
title.Parent = frame

-- Toggle GUI bằng logo
logo.MouseButton1Click:Connect(function()
    local existingUI = CoreGui:FindFirstChild("DucWareUI")
    if existingUI then
        existingUI:Destroy()
    else
        pcall(function()
            loadstring(game:HttpGet("https://raw.githubusercontent.com/ZeemMys/DucWare/main/cccc"))()
        end)
    end
end)

-- Mobile & Mouse drag
local dragging = false
local dragInput, mousePos, framePos

frame.InputBegan:Connect(function(input)
    if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
        dragging = true
        mousePos = input.Position
        framePos = frame.Position

        input.Changed:Connect(function()
            if input.UserInputState == Enum.UserInputState.End then
                dragging = false
            end
        end)
    end
end)

UIS.InputChanged:Connect(function(input)
    if dragging and (input.UserInputType == Enum.UserInputType.MouseMovement or input.UserInputType == Enum.UserInputType.Touch) then
        local delta = input.Position - mousePos
        frame.Position = UDim2.new(
            framePos.X.Scale, framePos.X.Offset + delta.X,
            framePos.Y.Scale, framePos.Y.Offset + delta.Y
        )
    end
end)
