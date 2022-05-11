﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	//sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	//worldTransform_[7].Initialize();


	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	
		//親(0番)
		worldTransform_[PartId::Root].Initialize();
		//子(1番)
		worldTransform_[PartId::Spine].translation_ = {0, 4.5f, 0};
		worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
		worldTransform_[PartId::Spine].Initialize();

		//上半身
		worldTransform_[PartId::Chest].translation_ = {0, 1.3f, 0};
		worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
		worldTransform_[PartId::Chest].Initialize();

		worldTransform_[PartId::Head].translation_ = {0, 3.0f, 0};
		worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
		worldTransform_[PartId::Head].Initialize();

		worldTransform_[PartId::ArmL].translation_ = {3.0f, 0, 0};
		worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
		worldTransform_[PartId::ArmL].Initialize();

		worldTransform_[PartId::ArmR].translation_ = {-3.0f, 0, 0};
		worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
		worldTransform_[PartId::ArmR].Initialize();

		//下半身
		worldTransform_[PartId::Hip].translation_ = {0, -2.0f, 0};
		worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
		worldTransform_[PartId::Hip].Initialize();

		worldTransform_[PartId::LegL].translation_ = {3.0f, -3.0f, 0};
		worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
		worldTransform_[PartId::LegL].Initialize();

		worldTransform_[PartId::LegR].translation_ = {-3.0f, -3.0f, 0};
		worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
		worldTransform_[PartId::LegR].Initialize();
	
	     viewProjection_.Initialize();
	//soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	/*audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);*/

	

	//// X Y Z軸周りの平行移動を設定
	//worldTransform_[1].translation_ = {0.0f, 4.5f, 0.0f};

	//for (size_t i = 0; i < _countof(worldTransform_); i++) {
	//	
	//	// X Y Z方向のスケーリングを設定
	//	worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};

	//	// X Y Z軸周りの回転角を
	//	worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};

	//	// X Y Z軸周りの平行移動を設定
	//	worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};
	//	//ワールドトランスフォームの初期化
	//	worldTransform_[i].Initialize();
	//}
	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();

	

	////カメラ視点座標を設定
	//viewProjection_.eye = {0, 0, -10};

	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();
	//
	////カメラ注視点座標を設定
	//viewProjection_.target = {10, 0, 0};

	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();

	////カメラ上方向ベクトルを設定(右上４５度指定)
	//viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};

	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();

	////カメラ垂直方向視野を設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();

	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離の設定
	//viewProjection_.farZ = 53.0f;

	////ビュープロジェクションの初期化
	//viewProjection_.Initialize();
}

void GameScene::Update() {

	//視点移動処理
	{
		//デバッグ再表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	//注視点移動処理
	{
		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグの再表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);
	}

	//上方向回転処理
	{
		//デバッグ用再表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}

	// Fov変更処理
	{
		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("forAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	}

	//クリップ距離変更処理
	{
		//デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	}

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		XMFLOAT3 move = {0, 0, 0};

		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		//押した方向で移送ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		//注視点移動
		worldTransform_[PartId::Root].translation_.x += move.x;
		worldTransform_[PartId::Root].translation_.y += move.y;
		worldTransform_[PartId::Root].translation_.z += move.z;

		worldTransform_[PartId::Root].UpdateMatrix();
		worldTransform_[PartId::Spine].UpdateMatrix();
		worldTransform_[PartId::Chest].UpdateMatrix();
		worldTransform_[PartId::Head].UpdateMatrix();
		worldTransform_[PartId::ArmL].UpdateMatrix();
		worldTransform_[PartId::ArmR].UpdateMatrix();
		worldTransform_[PartId::Hip].UpdateMatrix();
		worldTransform_[PartId::LegL].UpdateMatrix();
		worldTransform_[PartId::LegR].UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
		  "Root:(%f,%f,%f)", worldTransform_[PartId::Root].translation_.x,
		  worldTransform_[PartId::Root].translation_.y,
		  worldTransform_[PartId::Root].translation_.z);
	}

	//上半身回転処理
	{
		//上半身の回転の速さ{ラジアン/frame}
		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
		} else if (input_->PushKey(DIK_I)) {
			worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
		}
	}

	//下半身回転処理
	{
		//下半身の回転の速さ{ラジアン/frame}
		const float kHiptRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransform_[PartId::Hip].rotation_.y -= kHiptRotSpeed;
		} else if (input_->PushKey(DIK_K)) {
			worldTransform_[PartId::Hip].rotation_.y += kHiptRotSpeed;
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 3Dモデル描画
	/*model_->Draw(worldTransform_[PartId::Root], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Spine], viewProjection_, textureHandle_);*/
	model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHandle_);

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
