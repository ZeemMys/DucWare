local CoreGui = game:GetService("CoreGui")
local Players = game:GetService("Players")
local UIS = game:GetService("UserInputService")

-- GUI tạo launcher
local launcherGui = Instance.new("ScreenGui")
launcherGui.Name = "DucWareLauncher"
launcherGui.ResetOnSpawn = false
launcherGui.IgnoreGuiInset = true
launcherGui.Parent = CoreGui

-- Main button
local button = Instance.new("ImageButton")
button.Name = "ToggleButton"
button.Size = UDim2.new(0, 100, 0, 100)
button.Position = UDim2.new(0, 20, 0.5, -50)
button.BackgroundColor3 = Color3.fromRGB(30, 30, 30)
button.AutoButtonColor = false
button.Image = "rbxassetid://10172846277981" -- Logo anime bạn chọn
button.Parent = launcherGui

-- Bo góc
local corner = Instance.new("UICorner")
corner.CornerRadius = UDim.new(0, 100)
corner.Parent = button

-- Label tên DucWare
local title = Instance.new("TextLabel")
title.Name = "Title"
title.Size = UDim2.new(1, 0, 0, 20)
title.Position = UDim2.new(0, 0, 1, -5)
title.BackgroundTransparency = 1
title.Text = "DucWare"
title.Font = Enum.Font.GothamBold
title.TextSize = 14
title.TextColor3 = Color3.new(1, 1, 1)
title.TextStrokeTransparency = 0.5
title.Parent = button

-- Di chuyển bằng chuột hoặc tay
local dragging = false
local dragInput, startPos, startInputPos

button.InputBegan:Connect(function(input)
	if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
		dragging = true
		startInputPos = input.Position
		startPos = button.Position

		input.Changed:Connect(function()
			if input.UserInputState == Enum.UserInputState.End then
				dragging = false
			end
		end)
	end
end)

UIS.InputChanged:Connect(function(input)
	if dragging and (input.UserInputType == Enum.UserInputType.MouseMovement or input.UserInputType == Enum.UserInputType.Touch) then
		local delta = input.Position - startInputPos
		button.Position = UDim2.new(
			startPos.X.Scale, startPos.X.Offset + delta.X,
			startPos.Y.Scale, startPos.Y.Offset + delta.Y
		)
	end
end)

-- Toggle GUI khi click logo
button.MouseButton1Click:Connect(function()
	local existingGUI = CoreGui:FindFirstChild("DucWareUI")
	if existingGUI then
		existingGUI:Destroy()
	else
		pcall(function()
			loadstring(game:HttpGet("https://raw.githubusercontent.com/ZeemMys/DucWare/main/cccc"))()
		end)
	end
end)
