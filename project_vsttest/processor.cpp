// VST3 SDK�̃C���N���[�h�t�@�C��
#include "public.sdk/source/main/pluginfactoryvst3.h"

// ����VST�p�̃C���N���[�h�t�@�C��
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "processor.h"


// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
namespace Vst {

// =================================================================================
// �R���X�g���N�^
// =================================================================================
MyVSTProcessor::MyVSTProcessor()
{
	// �R���g���[���[��FUID��ݒ肷��
	setControllerClass(ControllerUID);
}

// ===================================================================================
// �N���X������������֐�
// ===================================================================================
tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context) {
	// �܂��p�����N���X�̏����������{
	tresult result = AudioEffect::initialize(context);
	if (result == kResultTrue) {
		// ���͂Əo�͂�ݒ�
		/*
		addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
		*/
		addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
		
		addEventInput(STR16("EventInput"), 1);

		// �ȉ��ŗL�̏����������{�B

		/*
		volume = 1.0f;
		depth = 1.0f;
		freq = 5.0f;
		type = 0;
		*/
		
		detune1 = 0.0f;
		gain1 = 0.0f;
		octave1 = 0;
		volume = 0.0f;

		thetaDebug = 0.5f;
		debugFlag = 0;

	}


	// �����������������kResultTrue��Ԃ��B
	return result;
}

tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) {
	// input��output�̃o�X��1���ŁA����input��output�̍\�����X�e���I�̏ꍇ
	if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo) {
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}

	// �Ή����Ă��Ȃ��o�X�\���̏ꍇ�AkResultFalse��Ԃ��B
	return kResultFalse;
}

// ===================================================================================
// �����M������������֐��i�o�b�t�@�������Ă��邲�ƂɎ��s�����j
// ===================================================================================
tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data){
	// �p�����[�^�[�ύX�̏���
	// �^����ꂽ�p�����[�^�[������Ƃ��Adata��inputParameterChanges��
	// IParameterChanges�N���X�ւ̃|�C���^�̃A�h���X������
	if (data.inputParameterChanges != NULL) {
		// �^����ꂽ�p�����[�^�[�̐����擾
		int32 paramChangeCount = data.inputParameterChanges->getParameterCount();

		// �^����ꂽ�p�����[�^�[���A�������J��Ԃ��B
		for (int32 i = 0; i < paramChangeCount; i++) {
			// �p�����[�^�[�ύX�̃L���[���擾
			// (��������T���v�����ɕ����̃p�����[�^�[�ύX��񂪂���\�������邽�߁A
			// �L���[�Ƃ����`�ɂȂ��Ă���B)
			IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
			if (queue != NULL) {
				// �ǂ̃p�����[�^�[���ύX���ꂽ���m�邽�߁A�p�����[�^�[tag���擾
				int32 tag = queue->getParameterId();

				// �ύX���ꂽ�񐔂��擾
				int32 valueChangeCount = queue->getPointCount();
				ParamValue value;
				int32 sampleOffset;

				// �Ō�ɕύX���ꂽ�l���擾
				if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
					// tag�ɉ��������������{
					switch (tag) {
					/*
					case PARAM_DEPTH_TAG:
						// depth��ύX����B
						depth = value;
						break;
					case PARAM_SPEED_TAG:
						// depth��ύX����B
						// RangeParameter�ō쐬���ꂽ�p�����[�^�[���A�v���Z�b�T�N���X�ɓn�����Ƃ���
						// 0.0�`1.0�ƂȂ��Ă��܂��B
						// �����ŊeRangeParameter�ɉ������͈͂�ݒ肷��K�v������B
						freq = (29.5f * value) + 0.5f; // 0.5�`30.0�̊ԂɕύX
						break;
					case PARAM_TYPE_TAG:
						// type��ύX����B
						// StringListParameter�ō쐬���ꂽ�p�����[�^�[���A�v���Z�b�T�N���X��
						// �n�����Ƃ���0.0�`1.0�ƂȂ��Ă��܂��B
						// ����̓��X�g����3�Ȃ̂ŁAVolume�c0.0f�ATremolo�c0.5f�APanning�c1.0f�ƂȂ�B
						// ���X�g�̐���4�̏ꍇ�A0.0f�A0.333�c�A0.666�c�A1.0f�ƂȂ�B
						// �u1.0f / (���X�g�̐� - 1)�v�ŋ��߂���B
						type = (int32)(value * 2.0f);
						break;
					*/
					case PARAM_OSC1_OCTAVE_TAG:
						// octave1��ύX����
						octave1 = (int32)(value * 6.0f);
						break;

					case PARAM_OSC1_DETUNE_TAG:
						// detune1��ύX����
						// 0�`100�ɐ��`�ϊ�
						detune1 = value * 100.0f;
						break;

					case PARAM_OSC1_GAIN_TAG:
						// gain1��ύX����
						gain1 = value;
						break;

					case PARAM_VOLUME_TAG:
						// volume��ύX����
						volume = value;
						break;
					}
				}
			}
		}
	}

	// ����MIDI�C�x���g�ɑ΂��鏈��
	IEventList* eventList = data.inputEvents;
	if (eventList != NULL) {
		// �C�x���g�̐����擾����B
		int32 numEvent = eventList->getEventCount();
		for (int32 i = 0; i < numEvent; i++) {
			// i�Ԗڂ̃C�x���g�f�[�^���擾����
			Event event;
			if (eventList->getEvent(i, event) == kResultOk) {
				int16 channel;
				int16 noteNo;
				float velocity;

				// �C�x���g�f�[�^�̃^�C�v���ƂɐU�蕪��
				switch (event.type) {
				case Event::kNoteOnEvent: // �m�[�g�I���C�x���g�̏ꍇ
					channel = event.noteOn.channel;
					noteNo = event.noteOn.pitch;
					velocity = event.noteOn.velocity;

					onNoteOn(channel, noteNo, velocity);
					break;

				case Event::kNoteOffEvent: // �m�[�g�I�t�C�x���g�̏ꍇ
					channel = event.noteOff.channel;
					noteNo = event.noteOff.pitch;
					velocity = event.noteOff.velocity;

					onNoteOff(channel, noteNo, velocity);
					break;
				}
			}
		}
	}

	// ���͉����M���ɑ΂��鏈��

	// ���́E�o�̓o�b�t�@�̃|�C���^���킩��₷���ϐ��Ɋi�[
	// inputs[]�Aoutputs[]��AudioBus�̐���������(addAudioInput()�AaddAudioOutput()�Œǉ�����������)
	// �����AudioBus��1�����Ȃ̂� 0 �݂̂ƂȂ�
	// channelBuffers32��32bit���������_�^�̃o�b�t�@�ŉ����M���̃`�����l����������
	// ���m����(kMono)�Ȃ� 0 �݂̂ŁA�X�e���I(kStereo)�Ȃ� 0(Left) �� 1(Right) �ƂȂ�
	/*
	Sample32* inL = data.inputs[0].channelBuffers32[0];
	Sample32* inR = data.inputs[0].channelBuffers32[1];
	*/
	Sample32* outL = data.outputs[0].channelBuffers32[0];
	Sample32* outR = data.outputs[0].channelBuffers32[1];
	
	std::vector<std::vector<float>> pitchList;
	for (int i = 0; i < noteList.size(); i++) {
		// �m�[�g�i���o�[�ƃI�N�^�[�u���璆�S���g�����v�Z����
		int note = noteList[i];
		float pitch = (440.0f * powf(2.0f, (float)(note - 69) / 12.0f) + (float)octave1);
		float factorMax = 1.0f + (0.01f / 12.00f) * detune1;
		float factorMin = 1.0f - (0.01f / 12.00f) * detune1;
		std::vector<float> subList(DETUNE_NUM);
		for (int j = 0; j < DETUNE_NUM; j++) {
			// detune���ꂽ���g�����v�Z����
			// �{����a���̍\�������Ƃɋ��߂Ă��Ė��ʂȂ̂ŉ��P�̗]�n�L��
			float detunedPitch = pitch * powf(2.0f,
				(factorMax * ((float)j) + factorMin * (float)(DETUNE_NUM-1-j)) / (float)(DETUNE_NUM-1));
			subList.push_back(detunedPitch);
		}
		pitchList.push_back(subList);

		// debug
		debugFlag = int(pitch);
	}

	// numSamples�Ŏ������T���v�����A��������������
	for (int32 i = 0; i < data.numSamples; i++) {
		/*
		// sin�֐��̌��ʂ�0�`1�̊Ԃɂ���(�g�������A�p���p)
		Sample32 a = (sin(theta) * 0.5f) + 0.5f;

		// depth��a������͐M���Ɋ|�����킹��l���v�Z����
		Sample32 b = (1.0f - depth) + (a * depth);
		Sample32 c = (1.0f - depth) + ((1.0f - a) * depth);

		switch (type) {
		case 0: // �{�����[���̏ꍇ
				// ���͐M����depth���|�����킹��(��قǂ�a��b�͖���)
			outL[i] = depth * inL[i];
			outR[i] = depth * inR[i];
			break;

		case 1: // �g�������̏ꍇ
				// ���͐M����b���|�����킹��(���E�������ʂɂ���̂�b�̂ݎg�p)
			outL[i] = b * inL[i];
			outR[i] = b * inR[i];
			break;

		case 2: // �p���̏ꍇ
				// ���͐M����b�Ac���|�����킹��(���E�ňقȂ鉹�ʂɂ���)
			outL[i] = b * inL[i];
			outR[i] = c * inR[i];
			break;
		}

		// �p�x�ƂɊp���x��������
		theta += (2.0f * 3.14159265f * freq) / 44100.0f;
		*/

		/*
		// �I�V���[�^�[�̉������擾����B
		float pitch = 0.0f;

		// �������X�g�̃T�C�Y��0�łȂ��ꍇ
		if (noteList.size() != 0) {
			// �Ō�ɉ����ꂽMIDI�m�[�g�̉������擾����
			pitch = noteList[noteList.size() - 1];
		}
		*/

		
		outL[i] = 0.0f;
		outR[i] = 0.0f;

		thetaDebug += 500.0f * (float)octave1 / 44100.0f;
		while (thetaDebug >= 1.0f) {
			thetaDebug -= 1.0f;
		}
		outL[i] += (thetaDebug - 0.5f) * 0.3f;
		outR[i] += (thetaDebug - 0.5f) * 0.3f;
		
		// debug
		
		// �e�����ɂ���
		for (int j = 0; j < pitchList.size(); j++) {
			for (int k = 0; k < DETUNE_NUM; k++) {
				float delta = pitchList[j][k] / 44100.0f;
				theta1[j][k] += delta;
				while (theta1[j][k] >= 1.0f) {
					theta1[j][k] -= 1.0f;
				}
				outL[i] += (theta1[j][k] - 0.5f) * 0.02f;
				outR[i] += (theta1[j][k] - 0.5f) * 0.02f;
			}
			//debugFlag = 2;
		}
		
		outL[i] *= volume;
		outR[i] *= volume;
		

		/*
		// �p�x�Ƃɉ����ɉ������p���x��������
		// (�����ł̓T���v�����[�g44100Hz�Œ�Ōv�Z���Ă���̂ŕK�v�ɉ����ď�������)
		theta += (2.0f * 3.14159265f * pitch) / 44100.0f;

		// �{�����[���ƃI�V���[�^(sin()�֐�)�̒l(�U��)���v�Z���ďo�͂���
		// (�ˑR�傫�ȉ����ł�Ƃ܂����̂łƂ肠����0.3f���|���Ă���)
		outL[i] = 0.3f * volume * sin(theta);
		outR[i] = 0.3f * volume * sin(theta);
		*/
	}

	// ���Ȃ����kResultTrue��Ԃ�(�����炭�K��kResultTrue��Ԃ�)
	return kResultTrue;
}
void MyVSTProcessor::onNoteOn(int channel, int note, float velocity)
{
	// MIDI�m�[�g�I���C�x���g�̏������s��

	// �ȒP�ȃT���v���Ȃ̂ŁAchannel��velocity�͖�������

	// �����ꂽ�m�[�g����A�������v�Z
	// �m�[�gNo.69��440Hz�ɂȂ�B�������Ɍv�Z����B
	// �v�Z���̏ڍא����ɂ��Ă͊���
	/*
	float pitch = (440.0f * powf(2.0f, (float)(note - (69)) / 12.0));
	*/

	// noteList�̍Ō�ɒǉ�����
	noteList.push_back(note);

	// �ʑ��e�[�u����ǉ�
	std::vector<float> th(DETUNE_NUM, 0.5f);
	theta1.push_back(th);

}

void MyVSTProcessor::onNoteOff(int channel, int note, float velocity)
{
	// MIDI�m�[�g�I�t�C�x���g�̏������s��

	// �ȒP�ȃT���v���Ȃ̂ŁAchannel��velocity�͖�������

	// �����ꂽ�m�[�g����A�������v�Z
	/*
	float pitch = (440.0f * powf(2.0f, (float)(note - (69)) / 12.0));
	*/

	// noteList���ŏ����猟�����Cnote�ɍ��v������̂��폜����
	for (int i = 0; i < (int)noteList.size(); i++) {
		if (noteList[i] == note) {
			// note�ƍ��v������̂��������ꍇ�A
			// �Y������f�[�^����菜���Č������I������
			noteList.erase(noteList.begin() + i);
			theta1.erase(theta1.begin() + i);
			break;
		}
	}

	/*
	// pitchList�̃T�C�Y��0�̏ꍇ�A�܂艟���ꂽ�m�[�g���Ȃ��ꍇ�A
	// �{�����[����0.0f�ɂ��ĉ�������
	if (noteList.size() == 0) {
		volume = 0.0f;
	}
	*/
}

}
} // namespace Steinberg��Vst�̏I���