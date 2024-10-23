#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>

// Function to load words from a file
std::vector<std::string> loadWordsFromFile(const std::string& fileName) {
   std::vector<std::string> wordList;
   std::ifstream file(fileName);
   std::string word;
   while (std::getline(file, word)) {
      wordList.push_back(word);
   }
   file.close();
   return wordList;
}

// Function to get a random word from the loaded list of words
std::string getRandomWord(const std::vector<std::string>& words) {
   srand(time(0));
   int index = rand() % words.size();
   return words[index];
}

void drawHangman(sf::RenderWindow& window, int mistakes) {
   // Draw different parts of the hangman depending on mistakes
   if (mistakes >= 1) {
      // Draw base
      sf::RectangleShape base(sf::Vector2f(200, 10));
      base.setPosition(50, 400);
      window.draw(base);
   }
   if (mistakes >= 2) {
      // Draw vertical pole
      sf::RectangleShape pole(sf::Vector2f(10, 300));
      pole.setPosition(100, 100);
      window.draw(pole);
   }
   if (mistakes >= 3) {
      // Draw top beam
      sf::RectangleShape beam(sf::Vector2f(150, 10));
      beam.setPosition(100, 100);
      window.draw(beam);
   }
   if (mistakes >= 4) {
      // Draw rope
      sf::RectangleShape rope(sf::Vector2f(10, 50));
      rope.setPosition(240, 110);
      window.draw(rope);
   }
   if (mistakes >= 5) {
      // Draw head
      sf::CircleShape head(30);
      head.setPosition(215, 160);
      window.draw(head);
   }
   if (mistakes >= 6) {
      // Draw body
      sf::RectangleShape body(sf::Vector2f(10, 100));
      body.setPosition(240, 215);
      window.draw(body);
   }
   if (mistakes >= 7) {
      // Draw left arm
      sf::RectangleShape leftArm(sf::Vector2f(50, 10));
      leftArm.setPosition(215, 220);
      leftArm.rotate(45);
      window.draw(leftArm);
   }
   if (mistakes >= 8) {
      // Draw right arm
      sf::RectangleShape rightArm(sf::Vector2f(50, 10));
      rightArm.setPosition(240, 255);
      rightArm.rotate(-45);
      window.draw(rightArm);
   }
   if (mistakes >= 9) {
      // Draw left leg
      sf::RectangleShape leftLeg(sf::Vector2f(50, 10));
      leftLeg.setPosition(205, 345);
      leftLeg.rotate(-45);
      window.draw(leftLeg);
   }
   if (mistakes >= 10) {
      // Draw right leg
      sf::RectangleShape rightLeg(sf::Vector2f(50, 10));
      rightLeg.setPosition(250, 310);
      rightLeg.rotate(45);
      window.draw(rightLeg);
   }
}

int main() {
   sf::RenderWindow window(sf::VideoMode(800, 600), "Hangman Game");
   sf::Font font;

   if (!font.loadFromFile("arial.ttf")) {
      std::cerr << "Could not load font\n";
      return -1;
   }

   // Load words from 'words.txt' file
   std::vector<std::string> words = loadWordsFromFile("words.txt");
   if (words.empty()) {
      std::cerr << "No words loaded from file!\n";
      return -1;
   }

   std::string wordToGuess = getRandomWord(words);
   std::string guessedLetters;
   int mistakes = 0;
   bool gameOver = false;

   // Set up game text elements
   sf::Text guessInput("Enter a letter:", font, 30);
   guessInput.setPosition(300, 150);
   guessInput.setFillColor(sf::Color::White);

   sf::Text resultMessage("", font, 75);
   resultMessage.setPosition(300, 300);
   resultMessage.setFillColor(sf::Color::Red);

   sf::Text revealWord("", font, 40); // Text to reveal the word on game over
   revealWord.setPosition(300, 400);
   revealWord.setFillColor(sf::Color::Yellow);

   sf::String playerInput;
   sf::Text inputDisplay("", font, 30);
   inputDisplay.setPosition(350, 200);
   inputDisplay.setFillColor(sf::Color::White);

   std::vector<sf::Text> letterDisplay(wordToGuess.length()); // Vector for individual letter/underscore

   // Set initial positions and properties for each underscore (letter)
   float startingPositionX = 100.0f;
   float yPosition = 500.0f;
   float spacing = 60.0f;  // Space between letters or underscores

   for (size_t i = 0; i < wordToGuess.length(); ++i) {
      letterDisplay[i].setFont(font);
      letterDisplay[i].setString("_");  // Initial display as underscore
      letterDisplay[i].setCharacterSize(50);
      letterDisplay[i].setFillColor(sf::Color::White);
      letterDisplay[i].setPosition(startingPositionX + i * spacing, yPosition); // Space them out
   }

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed)
            window.close();

         if (event.type == sf::Event::TextEntered && !gameOver) {
            if (event.text.unicode >= 'a' && event.text.unicode <= 'z') {
               char guess = static_cast<char>(event.text.unicode);

               if (guessedLetters.find(guess) == std::string::npos) {
                  guessedLetters += guess;
                  bool correctGuess = false;
                  for (size_t i = 0; i < wordToGuess.length(); i++) {
                     if (wordToGuess[i] == guess) {
                        letterDisplay[i].setString(guess);  // Replace underscore with the correct letter
                        correctGuess = true;
                     }
                  }

                  if (!correctGuess) {
                     mistakes++;
                  }

                  if (mistakes >= 10) {
                     resultMessage.setString("Game Over");
                     revealWord.setString("The word was: " + wordToGuess); // Reveal the word
                     gameOver = true;
                  }

                  bool allLettersRevealed = true;
                  for (size_t i = 0; i < wordToGuess.length(); ++i) {
                     if (letterDisplay[i].getString() == "_") {
                        allLettersRevealed = false;
                        break;
                     }
                  }

                  if (allLettersRevealed) {
                     resultMessage.setString("You Win!");
                     gameOver = true;
                  }
               }
            }
         }
      }

      window.clear();
      // Draw each letter or underscore individually
      for (const auto& letter : letterDisplay) {
         window.draw(letter);
      }
      window.draw(guessInput);
      inputDisplay.setString(guessedLetters);
      window.draw(inputDisplay);
      window.draw(resultMessage);

      // Display the revealed word if game is over
      if (gameOver && mistakes >= 10) {
         window.draw(revealWord); // Reveal the word only if the player loses
      }

      drawHangman(window, mistakes);
      window.display();
   }

   return 0;
}
