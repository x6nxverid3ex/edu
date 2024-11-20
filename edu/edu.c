#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

int main() {

    FILE* LogFile = fopen("log.txt", "a");

    time_t now = time(NULL);
    struct tm* current;
    current = localtime(&now);
    char formatted_time[30];
    strftime(formatted_time, sizeof(formatted_time), "%d.%m.%Y %H:%M:%S", current);
    fprintf(LogFile, "\n[%s] Program started.\n", formatted_time);

    char paramsPath[40];
    char outputFilePath[40];
    char fileExtension;


    printf("Enter the name of the parameters file:\n");
    gets_s(paramsPath, 40);
    strcat(paramsPath, ".bin");


    printf("Enter the name of the output file:\n");
    gets_s(outputFilePath, 40);

    // Choose the file format (text or binary)
    printf("Choose file extension: text (t) or binary (b):\n");
fileExtensionLabel:
    scanf_s(" %c", &fileExtension);
    if (fileExtension != 't' && fileExtension != 'b') goto fileExtensionLabel;

    switch (fileExtension) {
    case 't':
        strcat(outputFilePath, ".txt");
        break;
    case 'b':
        strcat(outputFilePath, ".bin");
        break;
    }


    double values[3] = { 0 };
    printf("Enter values for X, Y, and U:\n");
    for (size_t i = 0; i < 3; i++) {
        scanf_s("%lf", &values[i]);
    }


    FILE* paramsFile = fopen(paramsPath, "wb");
    fwrite(values, sizeof(double), 3, paramsFile);
    fclose(paramsFile);


    FILE* readParamsFile = fopen(paramsPath, "rb");
    double readValues[3];
    fread(readValues, sizeof(double), 3, readParamsFile);

    now = time(NULL);
    current = localtime(&now);
    strftime(formatted_time, sizeof(formatted_time), "%d.%m.%Y %H:%M:%S", current);
    fprintf(LogFile, "\n[%s] Parameters file [\"%s\"] opened. X=%lf Y=%lf U=%lf\n",
        formatted_time, paramsPath, readValues[0], readValues[1], readValues[2]);
    fclose(readParamsFile);


    double x = readValues[0], y = readValues[1], u = cos(x) + sin(x);  // U = cos(x) + sin(x)
    double Q = pow(u, (x + y) / 2.0) - cbrt((x - 1.0) / (fabs(y) + 1.0));

    now = time(NULL);
    current = localtime(&now);
    strftime(formatted_time, sizeof(formatted_time), "%d.%m.%Y %H:%M:%S", current);
    fprintf(LogFile, "\n[%s] Expression calculated. Result = %lf.\n", formatted_time, Q);


    FILE* outputFile = NULL;
    switch (fileExtension) {
    case 't':
        outputFile = fopen(outputFilePath, "wt");
        fprintf(outputFile, "X: %lf Y: %lf U: %lf - Result: %lf", x, y, u, Q);
        break;
    case 'b':
        outputFile = fopen(outputFilePath, "wb");
        double buffer[4] = { x, y, u, Q };
        fwrite(buffer, sizeof(double), 4, outputFile);
        break;
    }
    fclose(outputFile);

    now = time(NULL);
    current = localtime(&now);
    strftime(formatted_time, sizeof(formatted_time), "%d.%m.%Y %H:%M:%S", current);
    fprintf(LogFile, "\n[%s] Output file saved at [\"%s\"].\n", formatted_time, outputFilePath);
    fprintf(LogFile, "\n[%s] Program ended.\n", formatted_time);
    fclose(LogFile);

    return 0;
}
