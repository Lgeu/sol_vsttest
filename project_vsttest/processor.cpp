// VST3 SDKのインクルードファイル
#include "public.sdk/source/main/pluginfactoryvst3.h"

// 自作VST用のインクルードファイル
#include "myvst3define.h"
#include "myvst3fuid.h"
#include "processor.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg {
namespace Vst {

// =================================================================================
// コンストラクタ
// =================================================================================
MyVSTProcessor::MyVSTProcessor()
{
	// コントローラーのFUIDを設定する
	setControllerClass(ControllerUID);
}

// ===================================================================================
// クラスを初期化する関数
// ===================================================================================
tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context) {
	// まず継承元クラスの初期化を実施
	tresult result = AudioEffect::initialize(context);
	if (result == kResultTrue) {
		// 入力と出力を設定
		/*
		addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
		*/
		addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
		
		addEventInput(STR16("EventInput"), 1);

		// 以下固有の初期化を実施。

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


	// 初期化が成功すればkResultTrueを返す。
	return result;
}

tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) {
	// inputとoutputのバスが1つずつで、かつinputとoutputの構成がステレオの場合
	if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo) {
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}

	// 対応していないバス構成の場合、kResultFalseを返す。
	return kResultFalse;
}

// ===================================================================================
// 音声信号を処理する関数（バッファが送られてくるごとに実行される）
// ===================================================================================
tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data){
	// パラメーター変更の処理
	// 与えられたパラメーターがあるとき、dataのinputParameterChangesに
	// IParameterChangesクラスへのポインタのアドレスが入る
	if (data.inputParameterChanges != NULL) {
		// 与えられたパラメーターの数を取得
		int32 paramChangeCount = data.inputParameterChanges->getParameterCount();

		// 与えられたパラメーター分、処理を繰り返す。
		for (int32 i = 0; i < paramChangeCount; i++) {
			// パラメーター変更のキューを取得
			// (処理するサンプル内に複数のパラメーター変更情報がある可能性があるため、
			// キューという形になっている。)
			IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
			if (queue != NULL) {
				// どのパラメーターが変更されたか知るため、パラメーターtagを取得
				int32 tag = queue->getParameterId();

				// 変更された回数を取得
				int32 valueChangeCount = queue->getPointCount();
				ParamValue value;
				int32 sampleOffset;

				// 最後に変更された値を取得
				if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
					// tagに応じた処理を実施
					switch (tag) {
					/*
					case PARAM_DEPTH_TAG:
						// depthを変更する。
						depth = value;
						break;
					case PARAM_SPEED_TAG:
						// depthを変更する。
						// RangeParameterで作成されたパラメーターも、プロセッサクラスに渡されるときは
						// 0.0～1.0となってしまう。
						// 自分で各RangeParameterに応じた範囲を設定する必要がある。
						freq = (29.5f * value) + 0.5f; // 0.5～30.0の間に変更
						break;
					case PARAM_TYPE_TAG:
						// typeを変更する。
						// StringListParameterで作成されたパラメーターも、プロセッサクラスに
						// 渡されるときは0.0～1.0となってしまう。
						// 今回はリスト数は3つなので、Volume…0.0f、Tremolo…0.5f、Panning…1.0fとなる。
						// リストの数が4つの場合、0.0f、0.333…、0.666…、1.0fとなる。
						// 「1.0f / (リストの数 - 1)」で求められる。
						type = (int32)(value * 2.0f);
						break;
					*/
					case PARAM_OSC1_OCTAVE_TAG:
						// octave1を変更する
						octave1 = (int32)(value * 6.0f);
						break;

					case PARAM_OSC1_DETUNE_TAG:
						// detune1を変更する
						// 0～100に線形変換
						detune1 = value * 100.0f;
						break;

					case PARAM_OSC1_GAIN_TAG:
						// gain1を変更する
						gain1 = value;
						break;

					case PARAM_VOLUME_TAG:
						// volumeを変更する
						volume = value;
						break;
					}
				}
			}
		}
	}

	// 入力MIDIイベントに対する処理
	IEventList* eventList = data.inputEvents;
	if (eventList != NULL) {
		// イベントの数を取得する。
		int32 numEvent = eventList->getEventCount();
		for (int32 i = 0; i < numEvent; i++) {
			// i番目のイベントデータを取得する
			Event event;
			if (eventList->getEvent(i, event) == kResultOk) {
				int16 channel;
				int16 noteNo;
				float velocity;

				// イベントデータのタイプごとに振り分け
				switch (event.type) {
				case Event::kNoteOnEvent: // ノートオンイベントの場合
					channel = event.noteOn.channel;
					noteNo = event.noteOn.pitch;
					velocity = event.noteOn.velocity;

					onNoteOn(channel, noteNo, velocity);
					break;

				case Event::kNoteOffEvent: // ノートオフイベントの場合
					channel = event.noteOff.channel;
					noteNo = event.noteOff.pitch;
					velocity = event.noteOff.velocity;

					onNoteOff(channel, noteNo, velocity);
					break;
				}
			}
		}
	}

	// 入力音声信号に対する処理

	// 入力・出力バッファのポインタをわかりやすい変数に格納
	// inputs[]、outputs[]はAudioBusの数だけある(addAudioInput()、addAudioOutput()で追加した分だけ)
	// 今回はAudioBusは1つだけなので 0 のみとなる
	// channelBuffers32は32bit浮動小数点型のバッファで音声信号のチャンネル数分ある
	// モノラル(kMono)なら 0 のみで、ステレオ(kStereo)なら 0(Left) と 1(Right) となる
	/*
	Sample32* inL = data.inputs[0].channelBuffers32[0];
	Sample32* inR = data.inputs[0].channelBuffers32[1];
	*/
	Sample32* outL = data.outputs[0].channelBuffers32[0];
	Sample32* outR = data.outputs[0].channelBuffers32[1];
	
	std::vector<std::vector<float>> pitchList;
	for (int i = 0; i < noteList.size(); i++) {
		// ノートナンバーとオクターブから中心周波数を計算する
		int note = noteList[i];
		float pitch = (440.0f * powf(2.0f, (float)(note - 69) / 12.0f) + (float)octave1);
		float factorMax = 1.0f + (0.01f / 12.00f) * detune1;
		float factorMin = 1.0f - (0.01f / 12.00f) * detune1;
		std::vector<float> subList(DETUNE_NUM);
		for (int j = 0; j < DETUNE_NUM; j++) {
			// detuneされた周波数を計算する
			// 倍率を和音の構成音ごとに求めていて無駄なので改善の余地有り
			float detunedPitch = pitch * powf(2.0f,
				(factorMax * ((float)j) + factorMin * (float)(DETUNE_NUM-1-j)) / (float)(DETUNE_NUM-1));
			subList.push_back(detunedPitch);
		}
		pitchList.push_back(subList);

		// debug
		debugFlag = int(pitch);
	}

	// numSamplesで示されるサンプル分、音声を処理する
	for (int32 i = 0; i < data.numSamples; i++) {
		/*
		// sin関数の結果を0～1の間にする(トレモロ、パン用)
		Sample32 a = (sin(theta) * 0.5f) + 0.5f;

		// depthとaから入力信号に掛け合わせる値を計算する
		Sample32 b = (1.0f - depth) + (a * depth);
		Sample32 c = (1.0f - depth) + ((1.0f - a) * depth);

		switch (type) {
		case 0: // ボリュームの場合
				// 入力信号とdepthを掛け合わせる(先ほどのaとbは無視)
			outL[i] = depth * inL[i];
			outR[i] = depth * inR[i];
			break;

		case 1: // トレモロの場合
				// 入力信号とbを掛け合わせる(左右同じ音量にするのでbのみ使用)
			outL[i] = b * inL[i];
			outR[i] = b * inR[i];
			break;

		case 2: // パンの場合
				// 入力信号とb、cを掛け合わせる(左右で異なる音量にする)
			outL[i] = b * inL[i];
			outR[i] = c * inR[i];
			break;
		}

		// 角度θに角速度を加える
		theta += (2.0f * 3.14159265f * freq) / 44100.0f;
		*/

		/*
		// オシレーターの音程を取得する。
		float pitch = 0.0f;

		// 音程リストのサイズが0でない場合
		if (noteList.size() != 0) {
			// 最後に押されたMIDIノートの音程を取得する
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
		
		// 各音高について
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
		// 角度θに音程に応じた角速度を加える
		// (ここではサンプルレート44100Hz固定で計算しているので必要に応じて処理する)
		theta += (2.0f * 3.14159265f * pitch) / 44100.0f;

		// ボリュームとオシレータ(sin()関数)の値(振幅)を計算して出力する
		// (突然大きな音がでるとまずいのでとりあえず0.3fを掛けている)
		outL[i] = 0.3f * volume * sin(theta);
		outR[i] = 0.3f * volume * sin(theta);
		*/
	}

	// 問題なければkResultTrueを返す(おそらく必ずkResultTrueを返す)
	return kResultTrue;
}
void MyVSTProcessor::onNoteOn(int channel, int note, float velocity)
{
	// MIDIノートオンイベントの処理を行う

	// 簡単なサンプルなので、channelとvelocityは無視する

	// 押されたノートから、音程を計算
	// ノートNo.69が440Hzになる。これを基準に計算する。
	// 計算式の詳細説明については割愛
	/*
	float pitch = (440.0f * powf(2.0f, (float)(note - (69)) / 12.0));
	*/

	// noteListの最後に追加する
	noteList.push_back(note);

	// 位相テーブルを追加
	std::vector<float> th(DETUNE_NUM, 0.5f);
	theta1.push_back(th);

}

void MyVSTProcessor::onNoteOff(int channel, int note, float velocity)
{
	// MIDIノートオフイベントの処理を行う

	// 簡単なサンプルなので、channelとvelocityは無視する

	// 押されたノートから、音程を計算
	/*
	float pitch = (440.0f * powf(2.0f, (float)(note - (69)) / 12.0));
	*/

	// noteListを最初から検索し，noteに合致するものを削除する
	for (int i = 0; i < (int)noteList.size(); i++) {
		if (noteList[i] == note) {
			// noteと合致するものがあった場合、
			// 該当するデータを取り除いて検索を終了する
			noteList.erase(noteList.begin() + i);
			theta1.erase(theta1.begin() + i);
			break;
		}
	}

	/*
	// pitchListのサイズが0の場合、つまり押されたノートがない場合、
	// ボリュームを0.0fにして音を消す
	if (noteList.size() == 0) {
		volume = 0.0f;
	}
	*/
}

}
} // namespace SteinbergとVstの終わり