#include <stdio.h>

/* Function to compute required scores and weights */
void calculateRequiredScores(int firstExam, float firstExamWeight, int secondExam, float secondExamWeight, float homeworkAverage, float homeworkWeight, float finalExamWeight)
{

    // Initialize variables
    float partialGrade;
    char likelyGrade;
    
    // Calculate partial grades using the formula
    partialGrade = (firstExam * firstExamWeight) + (secondExam * secondExamWeight) + (homeworkAverage * homeworkWeight);
    
    // Calculate the grades needed for achieving A, B, C, and D
    int gradeNeededA = (int)(90 - partialGrade) / finalExamWeight;
    int gradeNeededB = (int)(80 - partialGrade) / finalExamWeight;
    int gradeNeededC = (int)(70 - partialGrade) / finalExamWeight;
    int gradeNeededD = (int)(60 - partialGrade) / finalExamWeight;

    // Calculate the likely score by averaging the two exams
    int likelyExamFinalScore = (firstExam + secondExam) / 2.0;

    // Calculate the final score based on the likely score
    int likelyFinalScore = (partialGrade + (likelyExamFinalScore * finalExamWeight)) / (firstExamWeight + secondExamWeight + homeworkWeight + finalExamWeight);

    // Determine the letter grade based on the final score
    if (likelyFinalScore >= 90 && likelyFinalScore <= 100)
    {
        likelyGrade = 'A';
    }
    else if (likelyFinalScore >= 80 && likelyFinalScore < 90)
    {
        likelyGrade = 'B';
    }
    else if (likelyFinalScore >= 70 && likelyFinalScore < 80)
    {
        likelyGrade = 'C';
    }
    else if (likelyFinalScore >= 60 && likelyFinalScore < 70)
    {
        likelyGrade = 'D';
    }
    else
    {
        likelyGrade = 'F';
    }

    // Print the results
    printf("\nBased on your test scores and homework grades, you will need to achieve:\n%d on your exam to get an A\n", gradeNeededA);
    printf("%d on your exam to get a B\n", gradeNeededB);
    printf("%d on your exam to get a C\n", gradeNeededC);
    printf("%d on your exam to get a D\n", gradeNeededD);
    

    // Print the likely score and associated grade 
    printf("\nIt is likely, based on your past performance, that you will achieve a %d on your exam, resulting in a final score of %d and a letter grade of %c ", likelyExamFinalScore, likelyFinalScore, likelyGrade);
}

int main()
{

    // Initialize variables
    int firstExam, secondExam;
    float firstExamWeight, secondExamWeight, homeworkAverage, homeworkWeight, finalExamWeight;

    // Obtain exam scores, homework average, and weight values from the user
    printf("Enter the score for the first exam : ");
    scanf("%d", &firstExam);
}