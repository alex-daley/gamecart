#include "CppUnitTest.h"
#include "game_service.h"
#include "mock_database.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(GameServiceTests)
	{
	public:
		TEST_METHOD(insert_validGame_insertAndFindRoundtripWorks)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());
			
			Game newGame;
			newGame.name = "NieR Replicant ver.1.22474487139…";
			newGame.genre = "Action";
			newGame.ageRating = 18;
			newGame.copies = 100;
			newGame.price = 40.0;

			gameService.insert(newGame);

			Game game = gameService.find(newGame.name);
			Assert::AreEqual(newGame.name, game.name);
			Assert::AreEqual(newGame.genre, game.genre);
			Assert::AreEqual(newGame.ageRating, game.ageRating);
			Assert::AreEqual(newGame.copies, game.copies);
			Assert::AreEqual(newGame.price, game.price);
		}

		TEST_METHOD(insert_duplicateGame_throwsRuntimeError)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Game newGame = { 0 };
			newGame.name = "Rocket League";

			gameService.insert(newGame);

			Assert::ExpectException<std::runtime_error>(
				[&]() {
					gameService.insert(newGame);
				}
			);
		}

		TEST_METHOD(decrementStock_validGame_decrementsStockCount)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Game newGame;
			newGame.name = "NieR Replicant ver.1.22474487139…";
			newGame.genre = "Action";
			newGame.ageRating = 18;
			newGame.copies = 100;
			newGame.price = 40.0;

			gameService.insert(newGame);
			int uid = gameService.find(newGame.name).uid;

			gameService.decrementStock(uid);
			Assert::AreEqual(99, gameService.find(newGame.name).copies);
		}

		TEST_METHOD(findByName_gameNotInDatabase_throwsRuntimeError)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Assert::ExpectException<std::runtime_error>(
				[&]() {
					gameService.find("Half Life 3");
				}
			);
		}

		TEST_METHOD(findByName_gameInDatabase_returnsCorrectGame)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Game newGame = { 0 };
			newGame.name = "Deus Ex: Mankind Divided";
			gameService.insert(newGame);

			Game game = gameService.find(newGame.name);
			Assert::AreEqual(newGame.name, game.name);
		}

		TEST_METHOD(findByID_gameNotInDatabase_throwsRuntimeError)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Assert::ExpectException<std::runtime_error>(
				[&]() {
					gameService.find(43895345);
				}
			);
		}

		TEST_METHOD(findByID_gameInDatabase_returnsCorrectGame)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Game newGame = { 0 };
			newGame.name = "Batman: Arkham City";
			gameService.insert(newGame);

			Game game = gameService.find(1);
			Assert::AreEqual(newGame.name, game.name);
		
		}

		TEST_METHOD(findAll_gamesInDatabase_returnsAllGamesInDatabase)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			std::string franchise = "Fifa";
			Game game = { 0 };

			std::vector<Game> games;

			for (std::size_t i = 0; i < 20; i++)
			{
				game.name = franchise + ": " + std::to_string(i);
				games.push_back(game);
				gameService.insert(game);
			}

			std::vector<Game> results = gameService.findAll();
			for (std::size_t i = 0; i < games.size(); i++)
			{
				Assert::AreEqual(games[i].name, results[i].name);
			}
		}

		TEST_METHOD(findAllWithGenreArgument_gamesInDatabase_returnsGamesInDatabaseOfSameGenre)
		{
			MockDatabase db;
			GameService gameService(db.getDatabase());

			Game platformer = { 0 };
			platformer.name = "Mario";
			platformer.genre = "Platformer";

			Game action1 = { 0 };
			action1.name = "Counterstrike";
			action1.genre = "Action";

			Game action2 = { 0 };
			action2.name = "Ratatouille";
			action2.genre = "Action";

			gameService.insert(platformer);
			gameService.insert(action1);
			gameService.insert(action2);

			std::vector<Game> actionGames = gameService.findAll("Action");
			Assert::AreEqual(2, (int)actionGames.size());
			
			for (const Game& game : actionGames)
			{
				Assert::AreEqual("Action", game.genre.c_str());
			}
		}
	};
}
