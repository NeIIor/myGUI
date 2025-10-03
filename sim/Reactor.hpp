// Reactor.hpp 
#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <vector>
#include <memory>
#include <random>
#include <deque>        
#include <functional>  
#include <cmath>       
#include <algorithm>    
#include "../../start/vector.hpp"


enum class MoleculeType {
    Round,
    Square
};

using Vector2f = Vector<float>;

class Molecule {
protected:
    Vector2f position;
    Vector2f velocity;
    float mass;
    MoleculeType type;

public:
    Molecule(MoleculeType t, float x, float y, float vx, float vy, float mass);
    virtual ~Molecule() = default;

    MoleculeType getType() const { return type; }
    Vector2f getPosition() const { return position; }
    Vector2f getVelocity() const { return velocity; }
    float getMass() const { return mass; }

    void setVelocity(float vx, float vy);
    void setPosition(float x, float y);
    
    virtual Vector2f getSize() const = 0;
    virtual void update(float dt) = 0;
    virtual bool collidesWith(const Molecule& other) const = 0;
};

class RoundMolecule : public Molecule {
private:
    float radius;

public:
    RoundMolecule(float x, float y, float vx, float vy, float radius, float mass = 1.0f);
    Vector2f getSize() const override;
    void update(float dt) override;
    bool collidesWith(const Molecule& other) const override;
    float getRadius() const { return radius; }
};

class SquareMolecule : public Molecule {
private:
    float size;

public:
    SquareMolecule(float x, float y, float vx, float vy, float size, float mass = 2.0f);
    Vector2f getSize() const override;
    void update(float dt) override;
    bool collidesWith(const Molecule& other) const override;
    float getSizeValue() const { return size; }
};

class Reactor {
private:
    std::vector<std::unique_ptr<Molecule>> molecules;
    std::vector<size_t> moleculesToRemove;
    
    std::mt19937 rng;
    std::uniform_real_distribution<float> distVel;

    int rightWallHitsLastSecond = 0;
    float hitTimer = 0.f;
    float leftWallTemperature = 1.0f;

    std::deque<int>   moleculeHistory;
    std::deque<int>   roundMoleculeHistory;
    std::deque<int>   squareMoleculeHistory;
    std::deque<float> energyHistory;
    std::deque<float> temperatureHistory;

    float reactorX, reactorY, reactorWidth, reactorHeight;
    float wallThickness;
    float moleculeRadius;
    float squareSize;
    float moleculeSpeed;

    void markForRemoval(size_t index);
    void processRemovals();
    static void handleRoundRoundCollision(Reactor& reactor, size_t i, size_t j, const Vector2f& collisionPos);
    static void handleRoundSquareCollision(Reactor& reactor, size_t i, size_t j, const Vector2f& collisionPos);
    static void handleSquareSquareCollision(Reactor& reactor, size_t i, size_t j, const Vector2f& collisionPos);

    using ReactionHandler = void(*)(Reactor&, size_t, size_t, const Vector2f&);
    ReactionHandler reactionTable[2][2];

    void updateMoleculePositions(float dt);
    void handleWallCollisions(Molecule& mol);
    void updateStatistics();

public:
    Reactor(float x, float y, float width, float height, float wallThick, 
            float molRadius, float sqSize, float molSpeed);

    void increaseLeftWallTemperature();
    void decreaseLeftWallTemperature();
    void addRoundMolecule();
    void addSquareMolecule();
    void handleCollisions();
    void removeLastMolecule();
    void addSquareMoleculeAt(float x, float y, float mass = 2.0f);
    void addRoundMoleculeAt(float x, float y, float vx, float vy);
    void resize(float newWidth);
    void update(float dt);
    void handleReaction(size_t i, size_t j);

    void clearAll() {
        molecules.clear();
        moleculesToRemove.clear();
        if (!moleculeHistory.empty()) {
            int last_count = moleculeHistory.back();
            moleculeHistory.clear();
            moleculeHistory.push_back(0);
            
            roundMoleculeHistory.clear();
            roundMoleculeHistory.push_back(0);
            
            squareMoleculeHistory.clear();
            squareMoleculeHistory.push_back(0);
            
            energyHistory.clear();
            energyHistory.push_back(0);
            
            temperatureHistory.clear();
            temperatureHistory.push_back(0);
        }
    }

    const std::vector<std::unique_ptr<Molecule>>& getMolecules() const { return molecules; }
    int getRightWallHits() const { return rightWallHitsLastSecond; }
    float getLeftWallTemperature() const { return leftWallTemperature; }
    
    const std::deque<int>& getMoleculeHistory() const { return moleculeHistory; }
    const std::deque<int>& getRoundMoleculeHistory() const { return roundMoleculeHistory; }
    const std::deque<int>& getSquareMoleculeHistory() const { return squareMoleculeHistory; }
    const std::deque<float>& getEnergyHistory() const { return energyHistory; }
    const std::deque<float>& getTemperatureHistory() const { return temperatureHistory; }
    
    float getReactorX() const { return reactorX; }
    float getReactorY() const { return reactorY; }
    float getReactorWidth() const { return reactorWidth; }
    float getReactorHeight() const { return reactorHeight; }
    float getWallThickness() const { return wallThickness; }
};

#endif