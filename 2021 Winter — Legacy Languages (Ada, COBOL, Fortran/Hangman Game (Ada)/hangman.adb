with Ada.Text_IO; use Ada.Text_IO;
with Ada.Characters.Handling; use Ada.Characters.Handling;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Strings.Unbounded.Text_IO; use Ada.Strings.Unbounded.Text_IO;
with Ada.Numerics.Discrete_Random; 
--with Ada.Characters.Conversions; use Ada.Characters.Conversions;
--with Ada.Characters.Handling; use Ada.Characters.Handling;



procedure Hangman is
    targetWord, foundLetters, prevGuesses: unbounded_string;
    targetLength : integer;
    input: string(1..1);
    numGuesses, fails: integer := 0;

    dictionarySize: constant integer := 50;
    dictionary: array (1..dictionarySize) of unbounded_string; 

    subtype randomRange is integer range 1..dictionarySize;
    package randomInt is new Ada.Numerics.Discrete_Random (randomRange);
    use randomInt;
    intGenerator: Generator;

    subtype line is string(1..12);
    gallows: array(1..12) of line;


    -- Prints opening message
    procedure startGame is
    begin
        new_line;
        put_line(" THE GAME OF HANGMAN");
        new_line;
    end startGame;


    -- Prints closing message
    procedure endGame is
    begin
        new_line;
        put_line(" Thanks for playing.");
        new_line;
    end endGame;


    -- Populates the dictionary array
    procedure defineDictionary is
    begin
        dictionary := (
            To_Unbounded_String("gum"),         To_Unbounded_String("sin"),         To_Unbounded_String("for"),
            To_Unbounded_String("cry"),         To_Unbounded_String("lug"),         To_Unbounded_String("bye"),
            To_Unbounded_String("fly"),         To_Unbounded_String("ugly"),        To_Unbounded_String("work"),
            To_Unbounded_String("each"),        To_Unbounded_String("from"),        To_Unbounded_String("talk"),
            To_Unbounded_String("with"),        To_Unbounded_String("self"),        To_Unbounded_String("kangaroo"),
            To_Unbounded_String("mahogany"),    To_Unbounded_String("sergeant"),    To_Unbounded_String("sequence"),
            To_Unbounded_String("pizza"),       To_Unbounded_String("thing"),       To_Unbounded_String("feign"),
            To_Unbounded_String("fiend"),       To_Unbounded_String("elbow"),       To_Unbounded_String("fault"),
            To_Unbounded_String("dirty"),       To_Unbounded_String("budget"),      To_Unbounded_String("spirit"),
            To_Unbounded_String("quaint"),      To_Unbounded_String("maiden"),      To_Unbounded_String("escort"),
            To_Unbounded_String("pickax"),      To_Unbounded_String("example"),     To_Unbounded_String("tension"),
            To_Unbounded_String("quinine"),     To_Unbounded_String("kidney"),      To_Unbounded_String("replica"),
            To_Unbounded_String("sleeper"),     To_Unbounded_String("triangle"),    To_Unbounded_String("moustache"),
            To_Unbounded_String("dangerous"),   To_Unbounded_String("scientist"),   To_Unbounded_String("different"),
            To_Unbounded_String("quiescent"),   To_Unbounded_String("magistrate"),  To_Unbounded_String("erroneously"),
            To_Unbounded_String("loudspeaker"), To_Unbounded_String("phytotoxic"),  To_Unbounded_String("matrimonial"),
            To_Unbounded_String("parasympathomimetic"),                             To_Unbounded_String("thigmotropism")
            );
    end defineDictionary;


    -- Returns a random integer
    function randomInteger return integer is
    begin
        reset(intGenerator);
        return (random(intGenerator));
    end randomInteger;


    -- Returns a word from the library using a random integer
    function getTargetWord(x: integer) return unbounded_string is
    begin
        return dictionary(x);
    end getTargetWord;


    -- Deletes everything in the prevGuesses unbounded string
    procedure initializePrevGuesses is
    lengthPrevGuesses: natural;
    begin
        lengthPrevGuesses := length(prevGuesses);
        delete(prevGuesses, 1, lengthPrevGuesses);
    end initializePrevGuesses;


    -- Deletes everything in the foundLetters unbounded string then 
    -- sets it up for the next word
    procedure initializeFoundLetters is
    lengthFoundLetters: natural;
    begin
        lengthFoundLetters := length(FoundLetters);
        delete(FoundLetters, 1, lengthFoundLetters);
        for i in 1..targetLength loop
            foundLetters := foundLetters & "-";
        end loop;
    end initializeFoundLetters;


    -- Fills gallows string array with spaces
    procedure initializeGallows is
    begin
        for i in 1..12 loop
            for j in 1..12 loop
                gallows(i)(j) := ' ';
            end loop;
        end loop;
    end initializeGallows;


    -- Prints the gallows string array
    procedure printGallows is 
    begin
        for i in 1..12 loop
            put_line(gallows(i));
        end loop;
        new_line;
    end printGallows;


    -- Updates gallows string array based on number of failures
    procedure updateGallows is 
    begin
        new_line;
        case fails is
        when 1 =>
        put_line(" First we draw a head.");
            for i in 1..12 loop
                gallows(i)(2) := 'X';
            end loop;
            for i in 2..8 loop
                gallows(1)(i) := 'X';
            end loop;
            gallows(2)(8) := 'X';
            for i in 7..9 loop
                gallows(3)(i) := '-';
                gallows(5)(i) := '-';
            end loop;
            gallows(4)(6) := '(';
            gallows(4)(7) := '.';
            gallows(4)(9) := '.';
            gallows(4)(10) := ')';
        when 2 =>
            put_line(" Now we draw a body.");
            for i in 6..9 loop
                gallows(i)(8) := 'X';
            end loop;
        when 3 =>
            put_line(" Next we draw an arm.");
            for i in 4..7 loop
                gallows(i)(i) := '\';
            end loop;
        when 4 =>
            put_line(" This time it's the other arm.");
            for i in 4..7 loop
                gallows(i)(16-i) := '/';
            end loop;
        when 5 =>
            put_line(" Now, let's draw the right leg.");
            for i in 10..11 loop
                gallows(i)(17-i) := '/';
            end loop;
        when 6 =>
            put_line(" This time we draw the left leg.");
            for i in 10..11 loop
                gallows(i)(i-1) := '\';
            end loop;
        when 7 =>
            put_line(" Now we put up a hand.");
            gallows(3)(12) := '\';
        when 8 =>
            put_line(" Next the other hand.");
            gallows(3)(4) := '/';
        when 9 =>
            put_line(" Now we draw one foot.");
            gallows(12)(12) := '-';
        when 10 =>
            put_line(" Here's the other foot -- You're hung!!.");
            gallows(12)(4) := '-';
        when others =>
            null;
    end case;
    end updateGallows;


    -- Prints letters that the user has found
    procedure displayFoundLetters is
    begin
        put_line(" " & foundLetters);
    end displayFoundLetters;


    -- Prints previous guesses from the user
    procedure displayPrevGuesses is
    begin
        put_line(" Here are the letters you used:");
        put_line(prevGuesses);
    end displayPrevGuesses;


    -- Adds a guessed character to the prevGuesses string
    procedure addPrevGuesses(guess: in character) is
    begin
        prevGuesses := prevGuesses & guess & " ";
    end addPrevGuesses;


    -- Prompts user for a guess and returns the first character entered
    function getGuess return character is
    guess: string(1..100);
    length: integer;
    begin
        put_line(" Please guess a letter: ");
        get_line(guess, length);
        return (to_lower(guess(1)));
    end getGuess;


    -- Tests if character is in string and returns true if found
    function isCharInString(str: string; length: integer; toFind: character) return boolean is
    begin
        for i in 1..length loop
            if (str(i) = toFind) then
                return true;
            end if;
        end loop;
        return false;
    end;


    -- Inserts most recently guessed character into foundLetters if that
    -- character is in the target word
    procedure updateFoundLetters is
    begin
        for i in 1..targetLength loop
            if (element(targetWord, i) = input(1)) then
                replace_element(foundLetters, i, input(1));
            end if;
        end loop;
    end updateFoundLetters;


    -- Prompts user to guess the target word, gets response, returns 
    -- true if it matches
    function guessWord return boolean is 
    inputWord: unbounded_string;
    begin
        put_line(" Please guess the word: ");
        get_line(inputWord);
        for i in 1..Ada.Strings.Unbounded.length(inputWord) loop
            replace_element(inputWord, i, to_lower(element(inputWord, i)));
        end loop;
        if (to_string(inputWord) = to_string(targetWord)) then
            return true;
        end if;
        return false;
    end guessWord;


    -- Tests whether an input character has been guessed previously,
    -- returns true if the character is new
    function newCharacter return boolean is
    begin
        for i in 1.. length(prevGuesses) loop
            if (element(prevGuesses, i) = input(1)) then
                return false;
            end if;
        end loop;
        return true;
    end newCharacter;


    -- Asks player if they wish to play again, gets input, tests that
    -- input is either 'y' or 'n', and returns true or false based on
    -- the input
    function playAgain return boolean is
    again: string(1..100);
    length: integer;
    begin
        loop
            put_line(" Do you want to play again? (Y/N?)");
            get_line(again, length);
            again(1) := to_lower(again(1));
            if (again(1) = 'y') then
                return true;
            elsif (again(1) = 'n') then
                return false;
            else
                put_line(" Please enter either Y or N.");
            end if;
        end loop;
    end playAgain;



    -- Runs the game
    begin
        startGame;
        defineDictionary;

        loop
            targetWord := getTargetWord(randomInteger);
            targetLength := length(targetWord);
            numGuesses := 0;
            fails := 0;
            initializeGallows;
            initializePrevGuesses;
            initializeFoundLetters;

            loop
                displayFoundLetters;
                displayPrevGuesses;
                input(1) := getGuess;
                numGuesses := numGuesses + 1;

                if (newCharacter) then
                    addPrevGuesses(input(1));

                    if (isCharInString(to_string(targetWord), targetLength, input(1))) then
                        updateFoundLetters;
                        displayFoundLetters;

                        if not (isCharInString(to_string(foundLetters), targetLength, '-')) then
                            put_line(" You found all the letters in" & integer'image(numguesses) & " guesses.");
                            exit;
                        end if;

                        if (guessWord) then
                            put_line("You found the word in" & integer'image(numguesses) & " guesses.");
                            exit;
                        end if;
                    else
                        fails := fails + 1;
                        put_line(" " & input & " is not in the word.");
                        updateGallows;
                        printGallows;
                    end if;

                    if (fails >= 10) then
                        put("Sorry, you lose. The word was ");
                        put_line(targetWord & ".");
                        exit;
                    end if;
                else
                    put_line("You have already guessed " & input(1));
                end if;
            end loop;

            if not (playAgain) then
                exit;
            end if;
        end loop;

        endGame;

    end Hangman;