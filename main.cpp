#include <Novice.h>
#include "MyMath/MatrixMath.h"
#include "MyMath/MyMath.h"

const char kWindowTitle[] = "LE2C_イシカワタケシ_MT3_01_01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	//クロス積確認用
	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);

	Vector3 rotate{0,0,0};
	Vector3 translate{0,0,0};
	//カメラ座標
	Vector3 cameraPos{ 0.0f,0.0f,-5.0f };

	Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPos);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	//射影行列
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	//ビューポート変換行列
	Matrix4x4 viewportMatrix = MakeVeiwportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);
	
	//三角形の頂点
	Vector3 screenVertices[3];
	Vector3 localVertices[3] = {
		{-0.5f,-0.5f,0.0f},
		{0.0f,0.5f,0.0f},
		{0.5f,-0.5f,0.0f}
	};
	Vector3 ndcVertex;

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);

		//三角形のY軸の回転
		rotate.y += 0.05f;

		//三角形の移動
		if (keys[DIK_W]) {
			translate.y += 0.01f;
		}

		if (keys[DIK_S]) {
			translate.y -= 0.01f;
		}

		if (keys[DIK_A]) {
			translate.x -= 0.01f;
		}

		if (keys[DIK_D]) {
			translate.x += 0.01f;
		}

		
		worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		for (uint32_t i = 0; i < 3; i++){
			ndcVertex = Transform(localVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//三角形の描画
		Novice::DrawTriangle(
			static_cast<int>(screenVertices[0].x),
			static_cast<int>(screenVertices[0].y),
			static_cast<int>(screenVertices[1].x),
			static_cast<int>(screenVertices[1].y),
			static_cast<int>(screenVertices[2].x),
			static_cast<int>(screenVertices[2].y),
			0xff0000ff,
			kFillModeSolid
		);


		//クロス積確認用
		VectorScreenPrintf(0, 0, cross, "cross");

		//debug
		Novice::ScreenPrintf(0, 20, "translate = %.02f,%.02f,%.02f", translate.x,translate.y,translate.z);
		Novice::ScreenPrintf(0, 40, "screenVertices = %.02f,%.02f,%.02f", screenVertices[0].x, screenVertices[0].y, screenVertices[0].z);
		Novice::ScreenPrintf(0, 60, "screenVertices = %.02f,%.02f,%.02f", screenVertices[1].x, screenVertices[1].y, screenVertices[1].z);
		Novice::ScreenPrintf(0, 80, "screenVertices = %.02f,%.02f,%.02f", screenVertices[2].x, screenVertices[2].y, screenVertices[2].z);
		
		
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
