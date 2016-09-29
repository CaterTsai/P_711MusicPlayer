#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	loadAllMusic();
	

	//Debug
	setupSimulation();
	//setupMidi();

	ofBackground(0);
	ofSetVerticalSync(true);

	_mainTimer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	float delta_ = ofGetElapsedTimef() - _mainTimer;
	_mainTimer += delta_;

	effectMusicMgr::GetInstance()->update(delta_);

	updateSimulation(delta_);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){

	drawSimulation();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	exitMidi();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch (key)
	{
	case 'a':
	{
		effectMusicMgr::GetInstance()->playBasic();
		effectMusicMgr::GetInstance()->play();
		_diskL.startMusic();
		_diskR.startMusic();
		break;
	}
	case 's':
	{
		effectMusicMgr::GetInstance()->stopBasic();
		effectMusicMgr::GetInstance()->stop();
		_diskL.stopMusic();
		_diskR.stopMusic();
		break;
	}
	//effect 
	case 'q':
	{
		bool play_ = (effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eLeft_Crash) == eStatePlay);
		(play_)? effectMusicMgr::GetInstance()->playerOut(eAudioType::eLeft_Crash) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eLeft_Crash);
		(play_) ? _diskL.channalOff(eAudioType::eLeft_Crash) : _diskL.channalOn(eAudioType::eLeft_Crash);
		break;
	}
	case 'w':
	{
		bool play_ = (effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eLeft_FXPad1) == eStatePlay);
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eLeft_FXPad1) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eLeft_FXPad1);
		(play_) ? _diskL.channalOff(eAudioType::eLeft_FXPad1) : _diskL.channalOn(eAudioType::eLeft_FXPad1);
		break;
	}
	case 'e':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eLeft_FXPad2) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eLeft_FXPad2) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eLeft_FXPad2);
		(play_) ? _diskL.channalOff(eAudioType::eLeft_FXPad2) : _diskL.channalOn(eAudioType::eLeft_FXPad2);
		break;
	}
	case 'r':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eLeft_Jingo) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eLeft_Jingo) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eLeft_Jingo);
		(play_) ? _diskL.channalOff(eAudioType::eLeft_Jingo) : _diskL.channalOn(eAudioType::eLeft_Jingo);
		break;
	}
	case 't':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eLeft_Toms) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eLeft_Toms) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eLeft_Toms);
		(play_) ? _diskL.channalOff(eAudioType::eLeft_Toms) : _diskL.channalOn(eAudioType::eLeft_Toms);
		break;
	}
	case 'y':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eRight_Bass) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eRight_Bass) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eRight_Bass);
		(play_) ? _diskR.channalOff(eAudioType::eRight_Bass) : _diskR.channalOn(eAudioType::eRight_Bass);
		break;
	}
	case 'u':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eRight_Drum) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eRight_Drum) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eRight_Drum);
		(play_) ? _diskR.channalOff(eAudioType::eRight_Drum) : _diskR.channalOn(eAudioType::eRight_Drum);
		break;
	}
	case 'i':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eRight_GT) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eRight_GT) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eRight_GT);
		(play_) ? _diskR.channalOff(eAudioType::eRight_GT) : _diskR.channalOn(eAudioType::eRight_GT);
		break;
	}
	case 'o':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eRight_LeadSynth) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eRight_LeadSynth) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eRight_LeadSynth);
		(play_) ? _diskR.channalOff(eAudioType::eRight_LeadSynth) : _diskR.channalOn(eAudioType::eRight_LeadSynth);
		break;
	}
	case 'p':
	{
		bool play_ = effectMusicMgr::GetInstance()->getPlayerState(eAudioType::eRight_Synth) == eStatePlay;
		(play_) ? effectMusicMgr::GetInstance()->playerOut(eAudioType::eRight_Synth) : effectMusicMgr::GetInstance()->playerIn(eAudioType::eRight_Synth);
		(play_) ? _diskR.channalOff(eAudioType::eRight_Synth) : _diskR.channalOn(eAudioType::eRight_Synth);
		break;
	}
	case 'z':
	{
		effectMusicMgr::GetInstance()->troggleEffect(eEffect_Echo);
		_buttons.troggleButton(eButton_Effect_Echo);
		break;
	}
	case 'x':
	{
		effectMusicMgr::GetInstance()->troggleEffect(eEffect_Distortion);
		_buttons.troggleButton(eButton_Effect_Dist);
		break;
	}
	case 'c':
	{
		effectMusicMgr::GetInstance()->troggleEffect(eEffect_PitchShift);
		_buttons.troggleButton(eButton_Effect_PitchShift);
		break;
	}
	}
}

#pragma region Effect Music Manager
//--------------------------------------------------------------
void ofApp::loadAllMusic()
{
	string audioPath_ = config::GetInstance()->audioPath;

	effectMusicMgr::GetInstance()->loadBasic(audioPath_ + "Basic.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Crash, audioPath_ + "L_CRASH.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_FXPad1, audioPath_ + "L_FX_PAD_01.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_FXPad2, audioPath_ + "L_FX_PAD_02.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Jingo, audioPath_ + "L_JINGO.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eLeft_Toms, audioPath_ + "L_Toms.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Bass, audioPath_ + "R_BASS.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Drum, audioPath_ + "R_DRUM SET.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_GT, audioPath_ + "R_GT.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_LeadSynth, audioPath_ + "R_LEAD_SYNTH.wav");
	effectMusicMgr::GetInstance()->addPlayer(eAudioType::eRight_Synth, audioPath_ + "R_SYNTH.wav");

	effectMusicMgr::GetInstance()->setup();
}

#pragma endregion

#pragma region Simulation
//--------------------------------------------------------------
void ofApp::setupSimulation()
{	
	ofSetCircleResolution(50);

	//Disk
	_diskL.setup(400);
	_diskL.addChannal(eLeft_Crash);
	_diskL.addChannal(eLeft_FXPad1);
	_diskL.addChannal(eLeft_FXPad2);
	_diskL.addChannal(eLeft_Jingo);
	_diskL.addChannal(eLeft_Toms);

	_diskR.setup(400);
	_diskR.addChannal(eRight_Bass);
	_diskR.addChannal(eRight_Drum);
	_diskR.addChannal(eRight_GT);
	_diskR.addChannal(eRight_LeadSynth);
	_diskR.addChannal(eRight_Synth);

	//Button
	_buttons.setup(400, 200, 2, 3);
	_buttons.addButton(eButton_Audio_1);
	_buttons.addButton(eButton_Audio_2);
	_buttons.addButton(eButton_Audio_3);
	_buttons.addButton(eButton_Effect_Echo);
	_buttons.addButton(eButton_Effect_Dist);
	_buttons.addButton(eButton_Effect_PitchShift);
}

//--------------------------------------------------------------
void ofApp::updateSimulation(float delta)
{
	_diskL.update(delta);
	_diskR.update(delta);
}

//--------------------------------------------------------------
void ofApp::drawSimulation()
{
	_diskL.draw(ofVec2f(ofGetWidth() / 4, ofGetHeight() / 2));
	_diskR.draw(ofVec2f(ofGetWidth() / 4 * 3, ofGetHeight() / 2));

	_buttons.draw(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2));
}
#pragma endregion

#pragma region Midi KB
//--------------------------------------------------------------
void ofApp::setupMidi()
{
	if (!_midiIn.openPort())
	{
		ofLog(OF_LOG_ERROR, "[MIDI] open midi in port failed");
		return;
	}

	//Mapper
	_midiMapper[0] = eLeft_Crash;
	_midiMapper[2] = eLeft_FXPad1;
	_midiMapper[4] = eLeft_FXPad2;
	_midiMapper[5] = eLeft_Jingo;
	_midiMapper[7] = eLeft_Toms;

	_midiMapper[16] = eRight_Bass;
	_midiMapper[17] = eRight_Drum;
	_midiMapper[19] = eRight_GT;
	_midiMapper[21] = eRight_LeadSynth;
	_midiMapper[23] = eRight_Synth;
	

	_midiIn.ignoreTypes(false, false, false);
	_midiIn.addListener(this);
}

//--------------------------------------------------------------
void ofApp::exitMidi()
{
	if (_midiIn.isOpen())
	{
		_midiIn.closePort();
		_midiIn.removeListener(this);
	}
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg)
{
	if (msg.status == MidiStatus::MIDI_NOTE_ON)
	{
		//Key press
		int node_ = msg.pitch % cMIDI_KEY_NUM;
		auto& Iter_ = _midiMapper.find(node_);
		if (Iter_ != _midiMapper.end())
		{
			effectMusicMgr::GetInstance()->playerIn(Iter_->second);
			
			if (Iter_->second < eLeft_Max)
			{
				_diskL.channalOn(Iter_->second);
			}
			else
			{
				_diskR.channalOn(Iter_->second);
			}
		}

	}
	else if (msg.status == MidiStatus::MIDI_NOTE_OFF)
	{
		//Key release
		int node_ = msg.pitch % cMIDI_KEY_NUM;

		auto& Iter_ = _midiMapper.find(node_);
		if (Iter_ != _midiMapper.end())
		{
			effectMusicMgr::GetInstance()->playerOut(Iter_->second);
			if (Iter_->second < eLeft_Max)
			{
				_diskL.channalOff(Iter_->second);
			}
			else
			{
				_diskR.channalOff(Iter_->second);
			}
		}
	}
}
#pragma endregion

