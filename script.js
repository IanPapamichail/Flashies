/*******************************************************
 *  Step 1: Fetch the JSON data for users & decks
 *******************************************************/
let users = []; // Initialize an empty array to store user data.
let decks = []; // Initialize an empty array to store deck data.

// Fetch user data from a JSON file named "users.json".
fetch("./users.json")
  .then(res => res.json()) // Convert the fetched response to JSON format.
  .then(data => {
    users = data.users; // Assign the user data from JSON to the 'users' array.
  })
  .catch(err => console.warn("users.json fetch error:", err)); // Log any errors that occur during fetching.

// Fetch deck data from a JSON file named "decks.json".
fetch("./decks.json")
  .then(res => res.json()) // Convert the fetched response to JSON format.
  .then(data => {
    decks = data.decks; // Assign the deck data from JSON to the 'decks' array.
  })
  .catch(err => console.warn("decks.json fetch error:", err)); // Log any errors that occur during fetching.

/*******************************************************
 *  Step 2: DOM Elements / Screens
 *******************************************************/
// Map of all screen DOM elements for easy access using their IDs.
const screens = {
  login: document.getElementById("login-screen"),
  signup: document.getElementById("signup-screen"),
  main: document.getElementById("main-screen"),
  study: document.getElementById("study-screen"),
  decks: document.getElementById("decks-screen"),
  deckEdit: document.getElementById("deck-edit-screen"),
  feedback: document.getElementById("feedback-screen"),
  progress: document.getElementById("progress-screen") // Newly added screen for progress display.
};

// Login elements
const loginUsername = document.getElementById("login-username");
const loginPassword = document.getElementById("login-password");
const loginBtn      = document.getElementById("login-btn");
const openSignup    = document.getElementById("open-signup");

// Signup elements
const signupUsername = document.getElementById("signup-username");
const signupPassword = document.getElementById("signup-password");
const signupBtnEl    = document.getElementById("signup-btn");
const openLogin      = document.getElementById("open-login");

// Main screen elements
const startStudyBtn    = document.getElementById("start-study-btn");
const feedbackBtn      = document.getElementById("feedback-btn");
const logoutBtn        = document.getElementById("logout-btn");
const viewProgressBtn  = document.getElementById("view-progress-btn"); // Button to view user progress.

// Study screen elements
const flashcardQuestion = document.getElementById("flashcard-question");
const flashcardAnswer   = document.getElementById("flashcard-answer");
const showAnswerBtn     = document.getElementById("show-answer-btn");
const nextCardBtn       = document.getElementById("next-card-btn");
const decksBtn          = document.getElementById("decks-btn");

// Decks screen elements
const deckListDiv   = document.getElementById("deck-list");
const addDeckBtn    = document.getElementById("add-deck-btn");
const backMainBtn   = document.getElementById("back-main-btn");

// Deck edit screen elements
const deckNameInput  = document.getElementById("deck-name-input");
const flashcardsList = document.getElementById("flashcards-list");
const saveDeckBtn    = document.getElementById("save-deck-btn");
const backDecksBtn   = document.getElementById("back-decks-btn");

// Feedback screen elements
const closeFeedbackBtn = document.getElementById("close-feedback-btn");

// Progress screen elements
const closeProgressBtn = document.getElementById("close-progress-btn");
const progressChartEl  = document.getElementById("progress-chart");

/*******************************************************
 *  Step 3: State
 *******************************************************/
// State variables to keep track of the current user, current deck, and all flashcards loaded.
let currentUser = null;
let currentDeck = null;
let allFlashcards = [];

/*******************************************************
 *  Step 4: Screen Switcher
 *******************************************************/
// Function to switch visibility between screens.
function switchScreen(screenEl) {
  for (let key in screens) {
    screens[key].classList.remove("active"); // Hide all screens.
    screens[key].style.display = "none";
  }
  screenEl.classList.add("active"); // Show the requested screen.
  screenEl.style.display = "block";
}

/*******************************************************
 *  Step 5: On Page Load, show login
 *******************************************************/
// Event listener to set the login screen as the initial view when the document is loaded.
document.addEventListener("DOMContentLoaded", () => {
  switchScreen(screens.login);
});

/*******************************************************
 *  Step 6: LOGIN / SIGNUP
 *******************************************************/
// Event listener for login button.
loginBtn.addEventListener("click", () => {
  const user = loginUsername.value.trim(); // Get and trim username input.
  const pass = loginPassword.value.trim(); // Get and trim password input.
  const found = users.find(u => u.username === user && u.password === pass); // Check if user exists and password matches.

  if (found) { // If credentials are valid,
    currentUser = found; // set the current user.
    alert(`Welcome, ${found.username}!`); // Welcome the user.
    switchScreen(screens.main); // Navigate to the main screen.
  } else {
    alert("Invalid credentials!"); // Alert if credentials are invalid.
  }
});

// Event listener for opening the signup screen.
openSignup.addEventListener("click", () => {
  switchScreen(screens.signup);
});

// Event listener for the signup button.
signupBtnEl.addEventListener("click", () => {
  const newUser = signupUsername.value.trim(); // Get and trim new username input.
  const newPass = signupPassword.value.trim(); // Get and trim new password input.

  if (!newUser || !newPass) { // Validate that fields are not empty.
    alert("Please fill all fields!"); // Alert if any field is empty.
    return;
  }

  const existing = users.find(u => u.username === newUser); // Check if the username already exists.
  if (existing) {
    alert("Username already exists!"); // Alert if username is taken.
    return;
  }

  // If username is new, add the user.
  users.push({ username: newUser, password: newPass });
  alert("Sign-up successful! You can now login."); // Confirm sign-up success.

  // Clear input fields and switch to the login screen.
  signupUsername.value = "";
  signupPassword.value = "";
  switchScreen(screens.login);
});

// Event listener for opening the login screen.
openLogin.addEventListener("click", () => {
  switchScreen(screens.login);
});

/*******************************************************
 *  Step 7: MAIN SCREEN
 *******************************************************/
// Event listener for the 'Start Studying' button.
startStudyBtn.addEventListener("click", () => {
  allFlashcards = gatherAllFlashcards(); // Aggregate all flashcards from all decks.
  if (allFlashcards.length === 0) { // Check if there are any flashcards available.
    alert("No flashcards available yet!"); // Alert if none are available.
    return;
  }
  switchScreen(screens.study); // Switch to the study screen if flashcards are available.
  loadRandomFlashcard(); // Load a random flashcard to start studying.
});

// Event listener for the 'Feedback' button.
feedbackBtn.addEventListener("click", () => {
  switchScreen(screens.feedback); // Switch to the feedback screen.
});

// Event listener for the 'Logout' button.
logoutBtn.addEventListener("click", () => {
  currentUser = null; // Clear the current user state.
  switchScreen(screens.login); // Switch back to the login screen.
});

// NEW: Event listener for the 'View Progress' button.
viewProgressBtn.addEventListener("click", () => {
  buildProgressChart(); // Build or update the progress chart.
  switchScreen(screens.progress); // Switch to the progress screen.
});

/*******************************************************
 *  Step 8: FEEDBACK SCREEN
 *******************************************************/
// Event listener for the 'Close' button on the feedback screen.
closeFeedbackBtn.addEventListener("click", () => {
  switchScreen(screens.main); // Return to the main screen.
});

/*******************************************************
 *  Step 9: STUDY SCREEN
 *******************************************************/
// Event listener for the 'Show Answer' button.
showAnswerBtn.addEventListener("click", () => {
  flashcardAnswer.classList.remove("hidden"); // Reveal the flashcard answer.
});

// Event listener for the 'Next Card' button.
nextCardBtn.addEventListener("click", () => {
  loadRandomFlashcard(); // Load the next random flashcard.
});

// Event listener for the 'Decks' button.
decksBtn.addEventListener("click", () => {
  switchScreen(screens.decks); // Switch to the decks screen.
  renderDeckList(); // Update and display the list of decks.
});
// Event listener for the 'I Know It' button.
const knowItBtn = document.getElementById("know-it-btn");
knowItBtn.addEventListener("click", () => {
  const idx = allFlashcards.findIndex(
    card => card.question === flashcardQuestion.textContent
  );
  if (idx !== -1) {
    allFlashcards.splice(idx, 1); // Remove the card from the study list.
  }
  if (allFlashcards.length > 0) {
    loadRandomFlashcard(); // Load another card.
  } else {
    alert("You have completed all cards in this session!");
    switchScreen(screens.main);
  }
});
// Event listener for the 'I Don't Know It' button.
const dontKnowItBtn = document.getElementById("dont-know-it-btn");
dontKnowItBtn.addEventListener("click", () => {
  loadRandomFlashcard(); // Keep the card in the study list but show the next one.
});
// Function to load a random flashcard.
function loadRandomFlashcard() {
  flashcardAnswer.classList.add("hidden"); // Hide the answer initially.
  const idx = Math.floor(Math.random() * allFlashcards.length); // Random index for flashcard.
  flashcardQuestion.textContent = allFlashcards[idx].question; // Display the question.
  flashcardAnswer.textContent = allFlashcards[idx].answer; // Prepare the answer.
}

/*******************************************************
 *  Step 10: DECKS SCREEN
 *******************************************************/
// Function to render the list of all decks.
function renderDeckList() {
  deckListDiv.innerHTML = ""; // Clear the existing deck list.
  decks.forEach(deck => {
    const div = document.createElement("div"); // Create a new div for each deck.
    div.className = "deck-item"; // Set class for styling.
    div.textContent = `${deck.name} (ID: ${deck.id})`; // Set text content with deck name and ID.

    // Event listener for each deck item.
    div.addEventListener("click", () => {
      if (confirm(`Deck: ${deck.name}\nOK=Edit, Cancel=Delete`)) { // Confirm dialog for editing or deleting.
        editDeck(deck); // Edit the deck if 'OK' is chosen.
      } else {
        decks = decks.filter(d => d.id !== deck.id); // Delete the deck if 'Cancel' is chosen.
        renderDeckList(); // Re-render the deck list.
      }
    });
    deckListDiv.appendChild(div); // Append the new div to the deck list container.
  });
}

// Event listener for the 'Add Deck' button.
addDeckBtn.addEventListener("click", () => {
  const deckName = prompt("Enter new deck name:"); // Prompt for a new deck name.
  if (!deckName) return; // If no name is entered, exit the function.
  const newId = generateId(); // Generate a new unique ID for the deck.
  decks.push({
    id: newId,
    name: deckName,
    cards: [] // Initialize an empty array for cards.
  });
  renderDeckList(); // Re-render the deck list with the new deck added.
});

// Event listener for the 'Back' button on the decks screen.
backMainBtn.addEventListener("click", () => {
  switchScreen(screens.main); // Return to the main screen.
});

/*******************************************************
 *  Step 11: DECK EDIT SCREEN
 *******************************************************/
// Function to edit a specific deck.
function editDeck(deck) {
  currentDeck = deck; // Set the currentDeck to the deck being edited.
  switchScreen(screens.deckEdit); // Switch to the deck edit screen.
  deckNameInput.value = deck.name; // Set the input field to the current deck name.
  renderDeckCards(deck.cards); // Render the list of cards in the deck.
}

// Function to render the list of cards in a deck.
function renderDeckCards(cards) {
  flashcardsList.innerHTML = ""; // Clear the existing list of cards.
  cards.forEach(card => {
    const div = document.createElement("div"); // Create a new div for each card.
    div.className = "flashcard-edit"; // Set class for styling.
    div.textContent = `Q: ${card.question} | A: ${card.answer}`; // Display the question and answer.

    // Event listener for each card.
    div.addEventListener("click", () => {
      if (confirm("Delete this card?")) { // Confirm dialog for deleting a card.
        currentDeck.cards = currentDeck.cards.filter(c => c.id !== card.id); // Delete the card if confirmed.
        renderDeckCards(currentDeck.cards); // Re-render the list of cards.
      }
    });
    flashcardsList.appendChild(div); // Append the new div to the card list container.
  });

  // "Add Card" button to add new cards to the deck.
  const btn = document.createElement("button");
  btn.textContent = "Add Card";
  btn.addEventListener("click", () => {
    const q = prompt("Card question?"); // Prompt for the question of the new card.
    const a = prompt("Card answer?"); // Prompt for the answer of the new card.
    if (q && a) {
      currentDeck.cards.push({ id: generateId(), question: q, answer: a }); // Add the new card to the deck.
      renderDeckCards(currentDeck.cards); // Re-render the list of cards.
    }
  });
  flashcardsList.appendChild(btn); // Append the "Add Card" button to the card list container.
}

// Event listener for the 'Save Deck' button.
saveDeckBtn.addEventListener("click", () => {
  if (!currentDeck) return; // If no deck is currently being edited, exit the function.
  currentDeck.name = deckNameInput.value; // Update the deck name with the value from the input field.
  alert("Deck updated!"); // Alert the user that the deck has been updated.
  switchScreen(screens.decks); // Return to the decks screen.
  renderDeckList(); // Re-render the deck list.
});

// Event listener for the 'Back' button on the deck edit screen.
backDecksBtn.addEventListener("click", () => {
  switchScreen(screens.decks); // Return to the decks screen.
});

/*******************************************************
 *  Step 12: Helpers
 *******************************************************/
// Function to gather all flashcards from all decks.
function gatherAllFlashcards() {
  let arr = []; // Initialize an empty array to store all flashcards.
  decks.forEach(d => {
    arr = arr.concat(d.cards); // Add all cards from each deck to the array.
  });
  return arr; // Return the array of all flashcards.
}

// Function to generate a unique ID for new decks or cards.
function generateId() {
  return Math.floor(Math.random() * 999999 + 100000); // Generate a random number between 100000 and 1099999.
}

/*******************************************************
 *  Step 13: PROGRESS SCREEN (Chart.js)
 *******************************************************/
// Event listener for the 'Close' button on the progress screen.
closeProgressBtn.addEventListener("click", () => {
  switchScreen(screens.main); // Return to the main screen.
});

// Function to build or update the progress chart using Chart.js.
function buildProgressChart() {
  // Calculate the total number of cards and the number of mastered cards.
  let total = 0;
  let mastered = 0;
  decks.forEach(deck => {
    deck.cards.forEach(card => {
      total++; // Increment the total count for each card.
      if (card.masteryLevel >= 3) { // Check if the card is considered mastered.
        mastered++; // Increment the mastered count.
      }
    });
  });

  // Prepare the data for the chart.
  const data = {
    labels: ["Mastered", "Not Mastered"], // Labels for the chart segments.
    datasets: [
      {
        label: "Progress", // Label for the dataset.
        data: [mastered, total - mastered], // Data points for the chart.
        backgroundColor: ["#1abc9c", "#e74c3c"] // Colors for the chart segments.
      }
    ]
  };

  // If a chart instance already exists, destroy it to avoid duplicates.
  if (window.progressChartInstance) {
    window.progressChartInstance.destroy();
  }

  const ctx = progressChartEl.getContext("2d"); // Get the drawing context for the chart.
  window.progressChartInstance = new Chart(ctx, {
    type: "doughnut", // Type of chart to create.
    data: data, // Data for the chart.
    options: {
      responsive: true // Make the chart responsive.
    }
  });
}
