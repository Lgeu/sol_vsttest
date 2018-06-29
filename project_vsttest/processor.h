#pragma once

// VST3 SDKのインクルードファイル
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h" // イベントバス利用時に必要

#include "myvst3define.h"

#include <random>

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
namespace Vst {

// ===================================================================================
// 音声信号を処理するProcessorクラス
// ===================================================================================
class MyVSTProcessor : public AudioEffect {
protected:
	/*
	ParamValue volume, depth, freq;
	int32 type;
	*/
	std::vector<int32> noteList;
	ParamValue volume, detune1, gain1;
	int32 octave1;
	std::vector<std::vector<float>> theta1;  // 各波の位相，[0,1)
	float thetaDebug;
	int debugFlag;

	// 乱数生成関連
	std::random_device rnd;
	std::mt19937 mt;
	std::uniform_real_distribution<float> randPhase;

public:
	// コンストラクタ
	MyVSTProcessor();

	// クラスを初期化する関数(必須)
	tresult PLUGIN_API initialize(FUnknown* context);

	// バス構成を設定する関数
	tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

	// 音声信号を処理する関数(必須)
	tresult PLUGIN_API process(ProcessData& data);

	// MIDIノートオンイベント、MIDIノートオフイベントを受け取った場合に処理する関数
	virtual void onNoteOn(int channel, int note, float velocity);
	virtual void onNoteOff(int channel, int note, float velocity);

	// 自作VST Processorクラスのインスタンスを作成するための関数(必須)
	static FUnknown* createInstance(void*) { return (IAudioProcessor*)new MyVSTProcessor(); }
};


}
} // namespace SteinbergとVstの終わり