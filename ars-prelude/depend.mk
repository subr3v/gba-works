AttackCommand.o: AttackCommand.cpp AttackCommand.h Command.h \
 GameConstants.h System.h Unit.h GameObject.h Point.h ObjectGraphics.h \
 UnitDescription.h GameController.h
CharacterMenuScene.o: CharacterMenuScene.cpp CharacterMenuScene.h Scene.h \
 GameConstants.h System.h character_menu_obj_sheet.h \
 character_menu_bg_sheet.h GameFonts.h FontDescription.h gba.h \
 GameScene.h Point.h DungeonMap.h GameRenderer.h DungeonGenerator.h \
 DungeonLevel.h SceneManager.h GraphicsUtil.h
CutsceneScene.o: CutsceneScene.cpp CutsceneScene.h Scene.h GameRenderer.h \
 Point.h System.h GameConstants.h GameFonts.h FontDescription.h \
 SceneManager.h ObjectGraphics.h gba.h GraphicsUtil.h intro_tileset.h \
 intro_tileset_fire.h intro_tileset_crystal.h GameWorld.h GameObject.h \
 Unit.h UnitDescription.h GameObjectFactory.h ItemDescription.h
DungeonGenerator.o: DungeonGenerator.cpp DungeonGenerator.h \
 GameConstants.h System.h DungeonMap.h GraphicsUtil.h
DungeonLevel.o: DungeonLevel.cpp DungeonLevel.h GameConstants.h System.h
DungeonMap.o: DungeonMap.cpp DungeonMap.h System.h DungeonGenerator.h \
 GameConstants.h
GameController.o: GameController.cpp GameController.h Point.h Item.h \
 GameObject.h GameConstants.h System.h ObjectGraphics.h Unit.h \
 UnitDescription.h ItemDescription.h GameWorld.h DungeonMap.h \
 GameRenderer.h GameFonts.h FontDescription.h gba.h
GameFonts.o: GameFonts.cpp GameFonts.h FontDescription.h System.h \
 normal_font.h outline_font.h GraphicsUtil.h gba.h
GameObject.o: GameObject.cpp GameObject.h Point.h GameConstants.h \
 System.h ObjectGraphics.h
GameObjectFactory.o: GameObjectFactory.cpp GameObjectFactory.h \
 GameConstants.h System.h UnitDescription.h ItemDescription.h Unit.h \
 GameObject.h Point.h ObjectGraphics.h Item.h
GameRenderer.o: GameRenderer.cpp GameRenderer.h Point.h System.h \
 GameConstants.h GameFonts.h FontDescription.h gba.h GraphicsUtil.h \
 GameObject.h ObjectGraphics.h DungeonMap.h ui_tileset.h obj_tileset.h \
 Unit.h UnitDescription.h Item.h ItemDescription.h tileset_dungeon.h \
 tileset_undersea.h tileset_lava.h
GameScene.o: GameScene.cpp GameScene.h Scene.h Point.h GameConstants.h \
 System.h DungeonMap.h GameRenderer.h GameFonts.h FontDescription.h \
 DungeonGenerator.h DungeonLevel.h gba.h GraphicsUtil.h Item.h \
 GameObject.h ObjectGraphics.h Unit.h UnitDescription.h ItemDescription.h \
 Gameworld.h GameController.h CharacterMenuScene.h SceneManager.h \
 GameObjectFactory.h Command.h IntroScene.h ShopScene.h CutsceneScene.h
GraphicsUtil.o: GraphicsUtil.cpp GraphicsUtil.h System.h gba.h
IntroScene.o: IntroScene.cpp IntroScene.h Scene.h System.h alcapa_logo.h \
 licensed.h ars_intro.h press_to_play.h GraphicsUtil.h gba.h GameScene.h \
 Point.h GameConstants.h DungeonMap.h GameRenderer.h GameFonts.h \
 FontDescription.h DungeonGenerator.h DungeonLevel.h SceneManager.h \
 CharacterMenuScene.h
Item.o: Item.cpp Item.h GameObject.h Point.h GameConstants.h System.h \
 ObjectGraphics.h Unit.h UnitDescription.h ItemDescription.h \
 GameController.h
Main.o: Main.cpp SceneManager.h IntroScene.h Scene.h GameScene.h Point.h \
 GameConstants.h System.h DungeonMap.h GameRenderer.h GameFonts.h \
 FontDescription.h DungeonGenerator.h DungeonLevel.h CharacterMenuScene.h \
 ShopScene.h CutsceneScene.h gba.h
MovementCommand.o: MovementCommand.cpp MovementCommand.h Command.h \
 GameConstants.h System.h Unit.h GameObject.h Point.h ObjectGraphics.h \
 UnitDescription.h Item.h ItemDescription.h GameController.h
ObjectGraphics.o: ObjectGraphics.cpp ObjectGraphics.h GameConstants.h \
 System.h sprites_sheet.h GraphicsUtil.h
SceneManager.o: SceneManager.cpp SceneManager.h Scene.h System.h
ShopScene.o: ShopScene.cpp ShopScene.h Scene.h GameConstants.h System.h \
 shop_menu_bg_sheet.h shop_menu_obj_sheet.h gba.h GraphicsUtil.h \
 SceneManager.h GameScene.h Point.h DungeonMap.h GameRenderer.h \
 GameFonts.h FontDescription.h DungeonGenerator.h DungeonLevel.h \
 GameController.h Unit.h GameObject.h ObjectGraphics.h UnitDescription.h
System.o: System.cpp System.h gba.h
Unit.o: Unit.cpp Unit.h GameObject.h Point.h GameConstants.h System.h \
 ObjectGraphics.h UnitDescription.h gba.h GameController.h GameWorld.h \
 MovementCommand.h Command.h AttackCommand.h WaitCommand.h \
 UseItemCommand.h GameObjectFactory.h ItemDescription.h
UseItemCommand.o: UseItemCommand.cpp UseItemCommand.h Command.h \
 GameConstants.h System.h GameController.h Point.h Unit.h GameObject.h \
 ObjectGraphics.h UnitDescription.h Item.h ItemDescription.h \
 GameObjectFactory.h
gba.o: gba.cpp gba.h
normal_font.o: normal_font.cpp normal_font.h
obj_tileset.o: obj_tileset.cpp obj_tileset.h
outline_font.o: outline_font.cpp outline_font.h
press_to_play.o: press_to_play.cpp press_to_play.h
