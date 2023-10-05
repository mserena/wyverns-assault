-- Module : Cutscene
-------------------------------------
-- Runs the a cut-scene
-- @return : true if the cut scene has ended, false otherwise
-------------------------------------

CameraMode_Game = 0
CameraMode_Fixed = 1
CameraMode_Free = 2
CameraMode_CutScene = 3

Game_Screen = 3

function playFinalCutScene(elapsedTime)
	local finished = false;

	-- Internal step position and play time
	local step = CutScene.getCurrentStep();
	local elapsedSceneTime = CutScene.getElapsedSceneTime();
	
	if step == 0 then
		-- Initialize cut scene
		Input.disable();
		Logic.disable(); -- Suspend enemy logic
		Physics.disable(); -- Suspend gae physics
		GameArea.disable(); -- Suspend game area update
		
		Gui.hideUi();
		Gui.showFrame();
		Gui.setFrame("Frame_Normal");
		
		local inputEnabled = Input.isEnabled(); -- Checks if input is enabled
		local logicEnabled = Logic.isEnabled(); -- Checks if logic is enabled
		local physicsEnabled = Physics.isEnabled(); -- Checks if physics are enabled
		
		Camera.setCurrent(CameraMode_CutScene); -- Sets the current camera	

		CutScene.nextStep(); -- Simply go to next step
	elseif step == 1 then
		if Camera.hasArrived() then	
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 2 then
		if CutScene.wait(5) then
			Audio.playOnce("PlayerYell01.wav");
			Gui.showForeground();
			Gui.setForeground("boss_2");
			CutScene.nextStep();
		end
	elseif step == 3 then
		-- Wait 3 seconds
		if CutScene.wait(2) then
			Gui.setForeground("boss_3");
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 4 then
		-- Wait 3 seconds
		if CutScene.wait(3) then
			Gui.setForeground("boss_4");
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 5 then
		-- Wait 3 seconds
		if CutScene.wait(3) then
			Audio.playOnce("WomanKiss01.wav");
			Gui.setForeground("boss_5");
			CutScene.nextStep(); -- Simply go to next step
		end
	elseif step == 6 then
		-- Wait 3 seconds
		if CutScene.wait(3) then
			CutScene.nextStep(); -- Simply go to next step
		end
	else
		Camera.setCurrent(CameraMode_Game); -- Sets the current camera
		
		Gui.hideFrame();
		Gui.hideForeground();
		Gui.showUi();
	
		-- scene
		Player.enable(); -- Resume player input
		Logic.enable(); -- Resume enemy logic
		Physics.enable(); -- Resume game physics
		GameArea.enable() -- Resume game area update
		Input.enable();
		
		CutScene.reset();		
		finished = true; -- Scene has ended
	end
		
	return finished;
end