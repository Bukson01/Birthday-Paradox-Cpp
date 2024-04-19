#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

// Function prototypes
void displayIntro();
std::vector<tm> generateBirthdays(int num_birthdays);
void displayBirthdays(const std::vector<tm>& birthdays);
tm findMatchingBirthday(const std::vector<tm>& birthdays);
void displayResults(const tm& match, int num_birthdays);

int main() {
    // Display the introduction message
    displayIntro();

    // Prompt the user for the number of birthdays to generate
    int num_birthdays;
    std::cout << "How many birthdays shall I generate? (Max 100): ";
    std::cin >> num_birthdays;

    // Validate the input
    while (std::cin.fail() || num_birthdays <= 0 || num_birthdays > 100) {
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Please enter a valid number between 1 and 100: ";
        std::cin >> num_birthdays;
    }

    // Generate birthdays
    std::vector<tm> birthdays = generateBirthdays(num_birthdays);

    // Display the generated birthdays
    displayBirthdays(birthdays);

    // Find matching birthday
    tm match = findMatchingBirthday(birthdays);

    // Display results
    displayResults(match, num_birthdays);

    return 0;
}

void displayIntro() {
    // Display the introduction message
    std::cout << "Birthday Paradox, by Al Sweigart al@inventwithpython.com\n\n"
                 "The Birthday Paradox shows us that in a group of N people, the odds\n"
                 "that two of them have matching birthdays is surprisingly large.\n"
                 "This program does a Monte Carlo simulation (that is, repeated random\n"
                 "simulations) to explore this concept.\n\n"
                 "(It's not actually a paradox, it's just a surprising result.)\n\n";
}

std::vector<tm> generateBirthdays(int num_birthdays) {
    // Generate random birthdays
    std::vector<tm> birthdays;
    std::time_t start_of_year = std::mktime(new std::tm{0, 0, 0, 1, 0, 101}); // January 1, 2001
    for (int i = 0; i < num_birthdays; ++i) {
        std::time_t random_time = start_of_year + (std::rand() % 365) * 86400; // 86400 seconds in a day
        birthdays.push_back(*std::localtime(&random_time));
    }
    return birthdays;
}

void displayBirthdays(const std::vector<tm>& birthdays) {
    // Display the generated birthdays
    std::cout << "Here are " << birthdays.size() << " birthdays:\n";
    for (size_t i = 0; i < birthdays.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << birthdays[i].tm_mon + 1 << "/" << birthdays[i].tm_mday;
    }
    std::cout << "\n\n";
}

tm findMatchingBirthday(const std::vector<tm>& birthdays) {
    // Find matching birthday
    std::vector<tm> sorted_birthdays = birthdays;
    std::sort(sorted_birthdays.begin(), sorted_birthdays.end(), 
              [](const tm& a, const tm& b) {
                  return std::tie(a.tm_mon, a.tm_mday) < std::tie(b.tm_mon, b.tm_mday);
              });
    for (size_t i = 1; i < sorted_birthdays.size(); ++i) {
        if (sorted_birthdays[i].tm_mon == sorted_birthdays[i - 1].tm_mon &&
            sorted_birthdays[i].tm_mday == sorted_birthdays[i - 1].tm_mday) {
            return sorted_birthdays[i];
        }
    }
    return {};
}

void displayResults(const tm& match, int num_birthdays) {
    // Display results
    std::cout << "In this simulation,";
    if (match.tm_mon != 0) {
        std::cout << " Multiple people have a birth on " << match.tm_mon + 1 << "/" << match.tm_mday << ".\n\n";
    } else {
        std::cout << " there are no matching birthdays.\n\n";
    }

    std::cout << "Generating " << num_birthdays << " random birthdays 100,000 times.\n";
    std::cout << "Press Enter to begin...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Let's run another 100,000 simulations.\n";

    int sim_match = 0;
    for (int i = 0; i < 100000; ++i) {
        if (i % 1000 == 0) {
            std::cout << i << " simulations run...\n";
        }
        std::vector<tm> birthdays = generateBirthdays(num_birthdays);
        if (findMatchingBirthday(birthdays).tm_mon != 0) {
            ++sim_match;
        }
    }
    std::cout << "100,000 simulations run.\n";

    double probability = static_cast<double>(sim_match) / 100000 * 100;
    std::cout << "Out of 100,000 simulations of " << num_birthdays << " people, there was a\n"
              << "matching birthday in that group " << sim_match << " times. This means\n"
              << "that " << num_birthdays << " people have a " << probability << "% chance of\n"
              << "having a matching birthday in their group.\n"
              << "That's probably more than you would think!\n";
}
