// сп - средних прямоугольников;

// пределы интегрирования, количество потоков (от 1 до 8), величина шага или количество шагов

// значение интеграла, время затраченное на вычисления. Подынтегральная функция выбирается самостоятельно.

#include <format>
#include <iostream>
#include <thread>
#include <vector>

#include <pthread.h>

#include <chrono>

#include <QApplication>

#include <Diagrams/diagrams.hpp>

/* ============================================
 * Вычисление интеграла
 * ============================================ */

struct CalcRes {
    double time;
    double value;
};

double func(double x) {
    return x*x*x + 23 * x - 20;
}

double step(double a, double b) {
    return func((a + b) / 2);
}

double process(double left, double stepSize, int stepCount) {
    double summ = 0.0;

    for (int i = 0; i < stepCount; ++i) {
        double start = left + i * stepSize;

        summ += step(start, start + stepSize);
    }

    summ *= stepSize;

    return summ;
}

/* ============================================
 * POSIX Threads
 * ============================================ */

struct ThreadData {
    double left;
    double stepSize;
    double result = 0;
    int stepCount;

    ThreadData(double left_, double stepSize_, int stepCount_)
        : left(left_), stepSize(stepSize_), stepCount(stepCount_) {}

    ThreadData() {}
};

void * worker( void * params ) {
    auto result = new double;

    ThreadData * data = ( ThreadData * ) params;

    *result = process(data->left, data->stepSize, data->stepCount);

    return result;
}

CalcRes pThreadsFunc(double start, double end, int stepCount, int threadsCount) {
    auto startTime = std::chrono::steady_clock::now();

    std::vector<pthread_t> threads(threadsCount);
    std::vector<ThreadData> threadsData(threadsCount);

    int countPerThread = stepCount / threadsCount;
    int rem = stepCount % threadsCount;

    double stepSize = (end - start) / stepCount;

    double last = start;

    for (int i = 0; i < threadsCount; ++i) {
        threadsData[i].stepCount = countPerThread + (i < rem ? 1 : 0);
        threadsData[i].left      = last;
        threadsData[i].stepSize  = stepSize;
        threadsData[i].result    = 0;

        last += threadsData[i].stepCount * stepSize;

        pthread_create(&threads[i], nullptr, worker, &threadsData[i]);
    }

    CalcRes result;

    result.value = 0.0;

    for (int i = 0; i < threadsCount; ++i) {
        double * res;

        pthread_join(threads[i], ( void * * ) &res);

        result.value += *res;

        delete res;
    }

    auto endTime = std::chrono::steady_clock::now();

    result.time = std::chrono::duration<double>(endTime - startTime).count();

    return result;
}

/* ============================================
 * STD Threads
 * ============================================ */

void stdWorker(double left, double stepSize, int stepCount, double * result) {
    *result = process(left, stepSize, stepCount);
}

CalcRes stdThreadsFunc(double start, double end, int stepCount, int threadsCount) {
    auto startTime = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(threadsCount);

    std::vector<double> results(threadsCount);

    int countPerThread = stepCount / threadsCount;
    int rem = stepCount % threadsCount;

    double stepSize = (end - start) / stepCount;

    double last = start;

    for (int i = 0; i < threadsCount; ++i) {
        int localStepCount = countPerThread + (i < rem ? 1 : 0);

        threads.emplace_back(stdWorker, last, stepSize, localStepCount, &results[i]);

        last += localStepCount * stepSize;
    }

    CalcRes result;

    result.value = 0.0;

    for (int i = 0; i < threadsCount; ++i) {
        threads[i].join();

        result.value += results[i];
    }

    auto endTime = std::chrono::steady_clock::now();

    result.time = std::chrono::duration<double>(endTime - startTime).count();

    return result;
}

/* Вычисляет с разным числом потоков, выводит данные в единном стиле, рисует диаграмму */
void calculate(double start, double end, int stepCount, CalcRes (function)(double start, double end, int stepCount, int threadsCount)) {
    std::vector<double> time;
    std::vector<std::string> labels;

    for (int i = 1; i <= 1024; i *= 2) {
        CalcRes result = function(start, end, stepCount, i);

        time.emplace_back(result.time);
        labels.emplace_back(std::format("T: {}", i));

        std::cout << "\n\nЧисло потоков: " << i << "\nРезультат: " << result.value << "\nВремя:" << result.time << std::endl;
    }

    Diagram::Diagram diagram;

    diagram.cleanSets();

    diagram.addSet("Время, С", time);

    diagram.setBarAxis(labels);

    diagram.show();
}

int main(int argc, char * * argv) {
    QApplication app(argc, argv);

    double start;
    double end;
    double stepCount;

    int result = 0;
    if (app.arguments().size() >= 4) {
        bool ok;
        int commulative = 0;
        start = app.arguments().at(1).toDouble(&ok);
        commulative += ok ? 1 : 0;
        end = app.arguments().at(2).toDouble(&ok);
        commulative += ok ? 1 : 0;
        stepCount = app.arguments().at(3).toInt(&ok);
        commulative += ok ? 1 : 0;
        // threadCount = app.arguments().at(4).toInt(&ok);
        // commulative += ok ? 1 : 0;

        if (commulative != 3) {
            result = -1;
        }
    } else {
        result = -1;
    }

    if (result == -1) {
        std::cout << "Error!\nExample usage:\n" << argv[0] << " 0.0 10.0 10000000" << std::endl;
        return -1;
    }

    calculate(start, end, stepCount, &pThreadsFunc);
    calculate(start, end, stepCount, &stdThreadsFunc);

    return 0;
}
