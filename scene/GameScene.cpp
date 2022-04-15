#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	/*audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);*/

	// X Y Z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	
	// X Y Z軸周りの回転角を設定
	worldTransform_.rotation_ = {XM_PI/4.0f,XM_PI/ 4.0f, 0.0f};
	
	// X Y Z軸周りの平行移動を設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	
}

void GameScene::Update() { XMFLOAT2 position = sprite_->GetPosition(); 
    position.x += 2.0f;
	position.y += 1.0f;
	debugText_->Print("translation:(10.000000,10.000000,10.000000)", 50, 50, 1.0f);
	debugText_->Print("rotation:(0.785398,0.785398,0.000000)", 50, 70, 1.0f);
	debugText_->Print("scale:(5.000000,5.000000,5.000000)", 50, 90, 1.0f);
	debugText_->SetPos(50,70);
	//debugText_->Printf("year:%d", 2001);
	sprite_->SetPosition(position);
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle_);
	}
	////変数の値をイクリメント
	//value_++;
	////値を含んだ文字列
	//std::string strDebug = std::string("Value:") + std::to_string(value_);
	////デバッグテキストの表示
	//debugText_->Print(strDebug, 50, 60, 1.0f);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	//</summary>
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
//#pragma region 背景スプライト描画
//	// 背景スプライト描画前処理
//	Sprite::PreDraw(commandList);
//
//	/// <summary>
//	/// ここに背景スプライトの描画処理を追加できる
//	/// </summary>
//	
//	sprite_->Draw();
//	// スプライト描画後処理
//	Sprite::PostDraw();
//	// 深度バッファクリア
//	dxCommon_->ClearDepthBuffer();
#pragma endregion

//#pragma region 3Dオブジェクト描画
//	// 3Dオブジェクト描画前処理
//	Model::PreDraw(commandList);
//
//	/// <summary>
//	/// ここに3Dオブジェクトの描画処理を追加できる
//	//</summary>
//	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
//
//	// 3Dオブジェクト描画後処理
//	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
