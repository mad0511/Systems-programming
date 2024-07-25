#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main()
{
    int curve[4];        // Array to hold curve values for each exam
    int examRange[4][2]; // Array to hold lower and upper bounds for each exam range

    // Get the no of students
    int NoS = 0;

    int last4StudentID, targetAverage;
    printf("Enter the last four digits of your student ID : ");
    scanf("%d", &last4StudentID);

    printf("\nEnter the target average for all subject : ");
    scanf("%d", &targetAverage);

    while (last4StudentID > 0)
    {
        NoS += last4StudentID % 10;
        last4StudentID /= 10;
    }
    NoS = (int)round((double)NoS / 4);
    printf("\nThe number of students are %d\n\n", NoS);

    // Allocate memory for the scores
    int **scores = (int **)malloc(NoS * sizeof(int *));
    for (int i = 0; i < NoS; i++)
    {
        scores[i] = (int *)malloc(4 * sizeof(int));
    }

    // Seed the random number generator
    srand(time(NULL));

    // Bonus
    // Get the exam score ranges from the user
    for (int i = 0; i < 4; i++)
    {
        do
        {
            printf("Enter the lower bound for Exam %d (between 60 and 100): ", i + 1);
            scanf("%d", &examRange[i][0]);
            printf("Enter the upper bound for Exam %d (between 60 and 100): ", i + 1);
            scanf("%d", &examRange[i][1]);
        } while (examRange[i][0] > examRange[i][1] || examRange[i][0] < 60 || examRange[i][1] > 100);
    }

    // Initialize test scores for each student based on (4xNoS)
    for (int i = 0; i < NoS; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            scores[j][i] = rand() % (examRange[j][1] - examRange[j][0] + 1) + examRange[j][0];
        }
    }

    // Calculate and display exam averages, apply curves if necessary
    for (int i = 0; i < 4; i++)
    {
        printf("\n");
        int total = 0;
        for (int j = 0; j < NoS; j++)
        {
            printf("\nThe student %d scores for subject %d : %d ", j + 1, i + 1, scores[i][j]);
            total += scores[i][j];
        }
        double average = (double)total / NoS;

        printf("\n\nThe average of the exam %d : %.2f\n", i + 1, average);
        if (average < targetAverage)
        {
            printf("Curve needed for Exam %d\n", i + 1);
            printf("Enter the curve value for Exam %d : ", i + 1);
            scanf("%d", &curve[i]);
            for (int j = 0; j < NoS; j++)
            {
                int curve_val = curve[i] - average;
                scores[i][j] += curve_val;
                printf("\nThe student %d scores for subject %d after adding a curve of %d : %d", j + 1, i + 1, curve_val, scores[i][j]);

                if (scores[i][j] > 100)
                {
                    scores[i][j] = 100; // Ensure scores do not exceed 100
                }
            }
        }
        else
        {
            printf("\nNo curve needed for Exam %d\n", i + 1);
        }
    }

    // Logic to calculate average marks of student after adding a curve
    for (int j = 0; j < NoS; j++)
    {
        int sumOfSubjects = 0;
        float finalAvg = 0;
        for (int i = 0; i < 4; i++)
        {
            sumOfSubjects += scores[i][j];
        }
        finalAvg = sumOfSubjects / 4;
        printf("\nStudent %d:%.2lf\n", j + 1, finalAvg);
    }

    printf("\nFreeing up memory allocation...");
    // Free the allocated memory
    for (int i = 0; i < NoS; i++)
    {
        free(scores[i]);
    }
    free(scores);
    return 0;
}