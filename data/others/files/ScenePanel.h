#pragma once
#include <BF/BlueFlame.h>
#include "Grid.h"
#include "TileSheetPanel.h"
#include "PaintTile.h"
#include "TileMap.h"

namespace Editor
{
	class ScenePanel : public BF::Graphics::GUI::Panel
	{
		private:
			BF::Math::Vector2 position;

			Grid grid;
			Editor::TileMap tilemap;
			PaintTile paintTile;
			TileSheetPanel &tileSheetPanel;

		public:
			ScenePanel(BF::Application::Scene& scene, TileSheetPanel& tileSheetPanel);
			~ScenePanel();

			void RenderTileMap();

			inline PaintTile& GetPaintTile() { return paintTile; }

		protected:
			void Initialize(BF::Graphics::Renderers::SpriteRenderer& spriteRenderer, int zLayer) override;
			void Load(const BF::Graphics::GUI::StyleSheet& StyleSheet, const std::string& widgetName) override;
			void Update() override;
			void Render() override;
	};
}