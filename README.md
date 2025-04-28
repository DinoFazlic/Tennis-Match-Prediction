# 🎾 Tennis Match Prediction

This C++ project predicts the probability that a tennis player will win a match, given the current match state and the serve probabilities. It uses dynamic programming to accurately model the rules of tennis, including games, sets, tie-breaks, and alternating servers.

---

## 🚀 Features

- **Accurate tennis modeling:** Takes into account current set/game/point scores, and who is serving.
- **Handles tie-breaks and normal games:** Uses probability and dynamic programming (DP) to calculate outcomes.
- **User input:** Interactive command-line interface for real-time scenarios.
- **Fast & reliable:** Handles deep match states quickly.

---

## 🛠️ How to Run

**Compile:**
g++ -o tennisMatchPrediction tennisMatchPrediction.cpp


**Run:**
tennisMatchPrediction

---

## 📋 How to Use

You’ll be prompted to enter:

- Probability that player 1 wins a point on their serve (e.g., 0.65)
- Probability that player 2 wins a point on their serve (e.g., 0.60)
- Number of sets won (by player 1, then player 2)
- Current set result: games won in current set (player 1, then player 2)
- Current gem result: points in current game (player 1, then player 2; use 0–3 for 0/15/30/40)
- Which player is serving first (1 or 2)

---

## 🧠 How It Works

- Uses dynamic programming to compute win probabilities from any match score.
- Models the entire match, including games, sets, and tie-breaks.
- Handles alternating servers and keeps the scoring realistic.

---

## ⚠️ Notes

- Do not enter probabilities as percentages! Use decimals (e.g., 0.65 for 65%).
- Points in the current gem: Use tennis convention: 0 (0), 1 (15), 2 (30), 3 (40).



