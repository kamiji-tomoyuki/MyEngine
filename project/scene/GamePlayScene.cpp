#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	// --- カメラ ---
	camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);

	// --- スプライト ---
	std::string textureFile[] = { "test/uvChecker.png","monsterBall.png" };
	for (uint32_t i = 0; i < 1; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(textureFile[i], { 0,0 }, { 1,1,1,1 }, { 0,0 });
		sprite->SetSize({ 100.0f,100.0f });

		sprites.push_back(sprite);
	}

	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("animation/AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("test.obj");

	for (uint32_t i = 0; i < 2; ++i) {
		Object3d* object = new Object3d();
		if (i == 0) {
			object->Initialize("AnimatedCube.gltf");
		}
		if (i == 1) {
			object->Initialize("test.obj");
		}
		
		Vector3 position;
		position.x = i * 2.0f;
		position.z = 2.0f;
		object->SetPosition(position);

		object3ds.push_back(object);
	}

	// --- オーディオ ---
	soundDataSet = Audio::GetInstance()->LoadWav("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataSet, false, 0.02f);

	soundDataSet2 = Audio::GetInstance()->LoadWav("test/xxx.wav");
	Audio::GetInstance()->PlayWave(soundDataSet2, false, 0.01f);
}

void GamePlayScene::Finalize()
{
	// 各解放処理
	delete camera;
	for (auto& obj : object3ds) {
		delete obj;
	}
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	Object3dCommon::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
}

void GamePlayScene::Update()
{
	//カメラの更新
	camera->Update();

#pragma region スプライト

	for (uint32_t i = 0; i < 1; ++i) {
		Vector2 position = { 200.0f * i, 0.0f };
		sprites[i]->SetPosition(position);

		float rotation = sprites[i]->GetRotate();
		sprites[i]->SetRotate(rotation);

		//Vector2 size = { 200.0f,200.0f };
		//sprites[i]->SetSize(size);

		Vector4 color = sprites[i]->GetColor();
		sprites[i]->SetColor(color);

		sprites[i]->Update();
	}

#pragma endregion スプライト

#pragma region 3Dオブジェクト

	for (uint32_t i = 0; i < object3ds.size(); ++i) {
		Object3d* obj = object3ds[i];
		obj->Update();
	}

#pragma endregion 3Dオブジェクト
}

void GamePlayScene::Draw()
{
#pragma region 背景 Sprite
	// ========== 背景 Sprite 描画 ==========
	// 描画前処理 (Sprite)
	SpriteCommon::GetInstance()->PreDraw();
	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓



	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
#pragma endregion

#pragma region Model
	// ==========    Model 描画    ==========
	// 描画前処理 (Object)
	Object3dCommon::GetInstance()->PreDraw();
	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

	for (auto& obj : object3ds) {
		obj->Draw();
	}

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
#pragma endregion

#pragma region 前景 Sprite
	// ========== 前景 Sprite 描画 ==========
	// 描画前処理 (Sprite)
	SpriteCommon::GetInstance()->PreDraw();

	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

	for (uint32_t i = 0; i < 1; ++i) {
		sprites[i]->Draw();
	}

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
#pragma endregion
}