// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(HiddenWords, *WordListPath);
    //TArray<FString> ValidWords = GetValidWords(HiddenWords);
    Isograms = GetValidWords(HiddenWords);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    //Check if lost
    //if game is not over check player guess
    if(!bGameOver)
    {
        ProcessGuess(Input);
    }
    //else clear screen and set game
    else
    {
        ClearScreen();
        //Reset if play again
        SetupGame();
    }
}
TArray<FString> UBullCowCartridge :: GetValidWords(const TArray<FString>& Words) const
{
    TArray<FString> ValidWords;
    for(FString Word : Words)
    {
        if(Word.Len()>= 4 && Word.Len()<= 8)
        {
            if(IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
    }
    return ValidWords;
}
void UBullCowCartridge :: SetupGame()
{
    //Welcome player
    PrintLine(TEXT("Hello there!"));
    PrintLine(TEXT("Welcome to Bull Cow Game"));
    
    //Choose hidden word out of list
    HiddenWord = Isograms[FMath :: RandRange(0, Isograms.Num()-1)];
    //Set lives
    Lives = HiddenWord.Len();
    //set text to accomodate for length of isogram
    //Print rules
    PrintLine(TEXT("Guess the %i-letter isogram!"), HiddenWord.Len());
    //Instruct player about length of word and prompt them to begin guessing
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in your guesses to play!"));
    //remove hard-coding and set word
    bGameOver = false;
}
void UBullCowCartridge :: EndGame()
{
    bGameOver = true;
    //Prompt to ask if play again
    PrintLine("Press enter to play again");
}
void UBullCowCartridge :: ProcessGuess (const FString& Guess)
{
    //Check for hidden word
    if( Guess == HiddenWord )
    {
        //If found, print victory and ask if they'd like to play again
        ClearScreen();
        PrintLine(TEXT("You win!"));
        EndGame();
        return;
    }
    //Check for Isogram
    if(!IsIsogram(Guess))
    {
        PrintLine("Your word has repeating characters, try again");
        return;
    }
    //Check for the right number of characters
    //if not then prompt them to enter right number of characters
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Your word has %i characters.\n The actual answer has %i characters!"), Guess.Len(), HiddenWord.Len());
        return;
    }
    PrintLine(TEXT("You lost a life!"));
    //print lives remaining
    PrintLine(TEXT("You have %i lives remaining"),--Lives);
    //Check if game lost via number of lives remaining
    if(Lives <= 0)
    {
        //If lost, print defeat and ask if they'd like to play again
        ClearScreen();
        PrintLine(TEXT("You lost!"));
        PrintLine(TEXT("The hidden word was %s."), *HiddenWord );//Debug line
        EndGame();
        return;
    }
    //Print number of cows and bulls
    FBullCowCount Count = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
    //  and ask players to enter again
    PrintLine(TEXT("Type to guess again"));
}
bool UBullCowCartridge :: IsIsogram(const FString& Word) const
{
    for(int32 i = 0; i < Word.Len() - 1; i++)
    {
        for(int32 j = i+1; j <Word.Len(); j++)
        {
            if(Word[j] == Word[i])
            {
                return false;
            }
        }
    }
    return true;
}
FBullCowCount UBullCowCartridge :: GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for(int32 i  = 0; i < Guess.Len(); i++)
    {
        if(Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            PrintLine(TEXT("HINT - Bull located on character %i of your guess!"), i);
            continue;
        }
        for(int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if(HiddenWord[j] == Guess[i] )
            {
                Count.Cows++;
                PrintLine(TEXT("HINT - Cow located on character %i of your guess!"), i);
                break;
            }
        }
    }
    return Count;
}