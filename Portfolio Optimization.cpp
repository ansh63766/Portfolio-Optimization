#include <bits/stdc++.h>

struct Asset {
    std::string name;
    double expectedReturn;
    double risk;
};

struct PortfolioMetrics {
    double portfolioReturn;
    double portfolioRisk;
    double sharpeRatio;
    double sortinoRatio;
    double maxDrawdown;
};

std::vector<double> optimizePortfolio(const std::vector<Asset>& assets, double targetReturn) {
    int numAssets = assets.size();
    std::vector<double> portfolioWeights(numAssets, 1.0 / numAssets);
    double portfolioReturn = 0.0;
    double portfolioRisk = 0.0;

    for (int i = 0; i < numAssets; i++) {
        portfolioReturn += portfolioWeights[i] * assets[i].expectedReturn;
        portfolioRisk += std::pow(portfolioWeights[i] * assets[i].risk, 2);
    }

    portfolioRisk = std::sqrt(portfolioRisk);
    if (portfolioReturn < targetReturn) {
        for (int i = 0; i < numAssets; i++) {
            double adjustedWeight = (assets[i].expectedReturn - portfolioReturn) / (targetReturn - portfolioReturn);
            portfolioWeights[i] = std::max(0.0, adjustedWeight);
        }
        double totalWeight = 0.0;
        for (int i = 0; i < numAssets; i++) {
            totalWeight += portfolioWeights[i];
        }

        for (int i = 0; i < numAssets; i++) {
            portfolioWeights[i] /= totalWeight;
        }
    }

    return portfolioWeights;
}

double calculatePortfolioValue(const std::vector<double>& portfolioWeights, const std::vector<Asset>& assets, double investment) {
    double portfolioValue = 0.0;

    for (int i = 0; i < assets.size(); i++) {
        double assetValue = portfolioWeights[i] * investment;
        portfolioValue += assetValue;
        std::cout << assets[i].name << " Value: " << assetValue << std::endl;
    }

    std::cout << "Portfolio Value: " << portfolioValue << std::endl;

    return portfolioValue;
}

double calculateSharpeRatio(double portfolioReturn, double portfolioRisk, double riskFreeRate) {
    return (portfolioReturn - riskFreeRate) / portfolioRisk;
}

double calculateSortinoRatio(double portfolioReturn, double portfolioRisk, double riskFreeRate, const std::vector<Asset>& assets) {
    double downsideRisk = 0.0;

    for (const Asset& asset : assets) {
        double assetReturn = asset.expectedReturn;
        double assetRisk = asset.risk;

        if (assetReturn < riskFreeRate) {
            downsideRisk += std::pow(assetRisk, 2);
        }
    }

    downsideRisk = std::sqrt(downsideRisk);

    if (downsideRisk == 0.0) {
        return 0.0;
    }

    return (portfolioReturn - riskFreeRate) / downsideRisk;
}

double calculateMaxDrawdown(const std::vector<double>& portfolioValues) {
    double maxDrawdown = 0.0;
    double peakValue = 0.0;

    for (double value : portfolioValues) {
        if (value > peakValue) {
            peakValue = value;
        } else {
            double drawdown = (peakValue - value) / peakValue;
            if (drawdown > maxDrawdown) {
                maxDrawdown = drawdown;
            }
        }
    }

    return maxDrawdown;
}

PortfolioMetrics analyzePortfolio(const std::vector<Asset>& assets, const std::vector<double>& portfolioWeights,
                                  double riskFreeRate, double investment) {
    PortfolioMetrics metrics;

    int numAssets = assets.size();
    std::vector<double> portfolioValues(numAssets, 0.0);

    double portfolioReturn = 0.0;
    double portfolioRisk = 0.0;

    for (int i = 0; i < numAssets; i++) {
        portfolioValues[i] = portfolioWeights[i] * investment;
        portfolioReturn += portfolioWeights[i] * assets[i].expectedReturn;
        portfolioRisk += std::pow(portfolioWeights[i] * assets[i].risk, 2);
    }

    portfolioRisk = std::sqrt(portfolioRisk);

    metrics.portfolioReturn = portfolioReturn;
    metrics.portfolioRisk = portfolioRisk;
    metrics.sharpeRatio = calculateSharpeRatio(portfolioReturn, portfolioRisk, riskFreeRate);
    metrics.sortinoRatio = calculateSortinoRatio(portfolioReturn, portfolioRisk, riskFreeRate, assets);
    metrics.maxDrawdown = calculateMaxDrawdown(portfolioValues);

    return metrics;
}

int main() {
    std::vector<Asset> assets = {
        {"Asset 1", 0.05, 0.1},
        {"Asset 2", 0.08, 0.15},
        {"Asset 3", 0.12, 0.18},
        {"Asset 4", 0.09, 0.12},
        {"Asset 5", 0.07, 0.14}
    };
    double targetReturn = 0.10;
    double investment = 10000.0;
    double riskFreeRate = 0.03;
    std::vector<double> portfolioWeights = optimizePortfolio(assets, targetReturn);
    std::cout << "Portfolio Weights:\n";
    for (int i = 0; i < assets.size(); i++) {
        std::cout << assets[i].name << ": " << portfolioWeights[i] << std::endl;
    }
    double portfolioValue = calculatePortfolioValue(portfolioWeights, assets, investment);
    PortfolioMetrics metrics = analyzePortfolio(assets, portfolioWeights, riskFreeRate, investment);
    std::cout << "Portfolio Metrics:\n";
    std::cout << "Portfolio Return: " << metrics.portfolioReturn << std::endl;
    std::cout << "Portfolio Risk: " << metrics.portfolioRisk << std::endl;
    std::cout << "Sharpe Ratio: " << metrics.sharpeRatio << std::endl;
    std::cout << "Sortino Ratio: " << metrics.sortinoRatio << std::endl;
    std::cout << "Max Drawdown: " << metrics.maxDrawdown << std::endl;

    return 0;
}
