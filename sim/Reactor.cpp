// Reactor.cpp
#include "Reactor.hpp"
#include <algorithm>
#include <cmath>

Molecule::Molecule(MoleculeType t, float x, float y, float vx, float vy, float mass) 
    : type(t), position(x, y, 0), velocity(vx, vy, 0), mass(mass) {}

void Molecule::setPosition(float x, float y) { 
    position = Vector2f(x, y, 0); 
}

void Molecule::setVelocity(float vx, float vy) { 
    velocity = Vector2f(vx, vy, 0); 
}

RoundMolecule::RoundMolecule(float x, float y, float vx, float vy, float radius, float mass) 
    : Molecule(MoleculeType::Round, x, y, vx, vy, mass), radius(radius) {}

Vector2f RoundMolecule::getSize() const {
    return Vector2f(radius * 2, radius * 2, 0);
}

void RoundMolecule::update(float dt) {
    position = position + velocity * dt;
}

bool RoundMolecule::collidesWith(const Molecule& other) const {
    if (other.getType() == MoleculeType::Round) {
        Vector2f pos1 = getPosition();
        Vector2f pos2 = other.getPosition();
        float distance = pos1 << pos2;
        const RoundMolecule* roundOther = dynamic_cast<const RoundMolecule*>(&other);
        if (roundOther) {
            return distance <= (radius + roundOther->radius);
        }
        return false;
    } else {
        Vector2f circleCenter = getPosition();
        Vector2f squareSize = other.getSize();
        Vector2f squarePos = other.getPosition();
        
        float closestX = std::max(squarePos.getX() - squareSize.getX()/2, 
                                std::min(circleCenter.getX(), squarePos.getX() + squareSize.getX()/2));
        float closestY = std::max(squarePos.getY() - squareSize.getY()/2, 
                                std::min(circleCenter.getY(), squarePos.getY() + squareSize.getY()/2));
        
        float dx = circleCenter.getX() - closestX;
        float dy = circleCenter.getY() - closestY;
        return (dx*dx + dy*dy) <= (radius * radius);
    }
}

SquareMolecule::SquareMolecule(float x, float y, float vx, float vy, float size, float mass) 
    : Molecule(MoleculeType::Square, x, y, vx, vy, mass), size(size) {}

Vector2f SquareMolecule::getSize() const {
    return Vector2f(size, size, 0);
}

void SquareMolecule::update(float dt) {
    position = position + velocity * dt;
}

bool SquareMolecule::collidesWith(const Molecule& other) const {
    if (other.getType() == MoleculeType::Round) {
        return other.collidesWith(*this);
    } else {
        Vector2f size1 = getSize();
        Vector2f pos1 = getPosition();
        Vector2f size2 = other.getSize();
        Vector2f pos2 = other.getPosition();
        
        return !(pos1.getX() - size1.getX()/2 > pos2.getX() + size2.getX()/2 ||
                 pos1.getX() + size1.getX()/2 < pos2.getX() - size2.getX()/2 ||
                 pos1.getY() - size1.getY()/2 > pos2.getY() + size2.getY()/2 ||
                 pos1.getY() + size1.getY()/2 < pos2.getY() - size2.getY()/2);
    }
}

Reactor::Reactor(float x, float y, float width, float height, float wallThick, 
                 float molRadius, float sqSize, float molSpeed)
    : reactorX(x), reactorY(y), reactorWidth(width), reactorHeight(height),
      wallThickness(wallThick), moleculeRadius(molRadius), 
      squareSize(sqSize), moleculeSpeed(molSpeed),
      rng(std::random_device{}()), distVel(-molSpeed, molSpeed) {
    reactionTable[0][0] = &Reactor::handleRoundRoundCollision;
    reactionTable[0][1] = &Reactor::handleRoundSquareCollision;
    reactionTable[1][0] = &Reactor::handleRoundSquareCollision;
    reactionTable[1][1] = &Reactor::handleSquareSquareCollision;
}

void Reactor::increaseLeftWallTemperature() {
    leftWallTemperature = std::min(leftWallTemperature + 0.2f, 3.0f); 
}

void Reactor::decreaseLeftWallTemperature() {
    leftWallTemperature = std::max(leftWallTemperature - 0.2f, 0.2f); 
}

void Reactor::addRoundMolecule() {
    float x = reactorX + wallThickness + moleculeRadius +
              (reactorWidth - 2 * wallThickness - 2 * moleculeRadius) * (float)rng() / rng.max();
    float y = reactorY + wallThickness + moleculeRadius +
              (reactorHeight - 2 * wallThickness - 2 * moleculeRadius) * (float)rng() / rng.max();
    float vx = distVel(rng);
    float vy = distVel(rng);
    molecules.push_back(std::make_unique<RoundMolecule>(x, y, vx, vy, moleculeRadius));
}

void Reactor::addSquareMolecule() {
    float x = reactorX + wallThickness + squareSize/2 +
              (reactorWidth - 2 * wallThickness - squareSize) * (float)rng() / rng.max();
    float y = reactorY + wallThickness + squareSize/2 +
              (reactorHeight - 2 * wallThickness - squareSize) * (float)rng() / rng.max();
    float vx = distVel(rng);
    float vy = distVel(rng);
    molecules.push_back(std::make_unique<SquareMolecule>(x, y, vx, vy, squareSize, 2.0f));
}

void Reactor::addSquareMoleculeAt(float x, float y, float mass) {
    float vx = distVel(rng);
    float vy = distVel(rng);
    molecules.push_back(std::make_unique<SquareMolecule>(x, y, vx, vy, squareSize, mass));
}

void Reactor::addRoundMoleculeAt(float x, float y, float vx, float vy) {
    molecules.push_back(std::make_unique<RoundMolecule>(x, y, vx, vy, moleculeRadius));
}

void Reactor::removeLastMolecule() {
    if (!molecules.empty()) {
        molecules.pop_back();
    }
}

void Reactor::resize(float newWidth) {
    reactorWidth = newWidth;
}

void Reactor::update(float dt) {
    hitTimer += dt;
    if (hitTimer >= 1.0f) {
        hitTimer = 0;
        rightWallHitsLastSecond = 0;
    }

    updateMoleculePositions(dt);
    handleCollisions();
    processRemovals();
    updateStatistics();
}

void Reactor::handleCollisions() {
    std::vector<std::pair<size_t, size_t>> collisions;
    
    for (size_t i = 0; i < molecules.size(); ++i) {
        if (!molecules[i]) continue;
        for (size_t j = i + 1; j < molecules.size(); ++j) {
            if (molecules[j] && molecules[i]->collidesWith(*molecules[j])) {
                collisions.emplace_back(i, j);
                break; 
            }
        }
    }
    
    for (auto it = collisions.rbegin(); it != collisions.rend(); ++it) {
        handleReaction(it->first, it->second);
    }
}

void Reactor::handleReaction(size_t i, size_t j) {
    if (i >= molecules.size() || j >= molecules.size() || i == j || !molecules[i] || !molecules[j]) return;
    
    size_t type1 = static_cast<size_t>(molecules[i]->getType());
    size_t type2 = static_cast<size_t>(molecules[j]->getType());
    Vector2f pos1 = molecules[i]->getPosition();
    Vector2f pos2 = molecules[j]->getPosition();

    Vector2f collisionPos = (pos1 + pos2) * 0.5f;

    ReactionHandler handler = reactionTable[type1][type2];
    if (handler) {
        handler(*this, i, j, collisionPos);
    }
}

void Reactor::handleRoundRoundCollision(Reactor& reactor, size_t i, size_t j, const Vector2f& collisionPos) {
    Vector2f vel1 = reactor.molecules[i]->getVelocity();
    Vector2f vel2 = reactor.molecules[j]->getVelocity();
    
    Vector2f vel = (vel1 + vel2) * 0.5f;
    
    reactor.markForRemoval(i);
    reactor.markForRemoval(j);
    
    reactor.addSquareMoleculeAt(collisionPos.getX(), collisionPos.getY(), 2.0f);
    reactor.molecules.back()->setVelocity(vel.getX(), vel.getY());
}

void Reactor::markForRemoval(size_t index) {
    moleculesToRemove.push_back(index);
}

void Reactor::processRemovals() {
    std::sort(moleculesToRemove.rbegin(), moleculesToRemove.rend());
    moleculesToRemove.erase(std::unique(moleculesToRemove.begin(), moleculesToRemove.end()), moleculesToRemove.end());
    
    for (auto index : moleculesToRemove) {
        if (index < molecules.size()) {
            molecules.erase(molecules.begin() + index);
        }
    }
    moleculesToRemove.clear();
}

void Reactor::handleRoundSquareCollision(Reactor& reactor, size_t i, size_t j, const Vector2f& collisionPos) {
    size_t squareIdx, roundIdx;
    
    if (reactor.molecules[i]->getType() == MoleculeType::Square) {
        squareIdx = i;
        roundIdx  = j;
    } else {
        squareIdx = j;
        roundIdx  = i;
    }

    float    squareMass = reactor.molecules[squareIdx]->getMass    ();
    float    roundMass  = reactor.molecules[roundIdx ]->getMass    (); 
    
    Vector2f squareVel  = reactor.molecules[squareIdx]->getVelocity();
    Vector2f roundVel   = reactor.molecules[roundIdx ]->getVelocity();
    
    Vector2f totalMomentum = squareVel * squareMass + roundVel * roundMass;
    float newMass = squareMass + roundMass;
    Vector2f newVel = totalMomentum * (1.0f / newMass); 
    
    float x = reactor.molecules[squareIdx]->getPosition().getX();
    float y = reactor.molecules[squareIdx]->getPosition().getY();
    
    reactor.markForRemoval(roundIdx);
    
    if (roundIdx < squareIdx) {
        squareIdx--;
    }
    
    reactor.molecules[squareIdx] = std::make_unique<SquareMolecule>(x, y, newVel.getX(), newVel.getY(), reactor.squareSize, newMass);
}

void Reactor::handleSquareSquareCollision(Reactor& reactor, size_t i, size_t j, const Vector2f& collisionPos) {
    float mass1 = reactor.molecules[i]->getMass();
    float mass2 = reactor.molecules[j]->getMass();
    
    int numNewMolecules = static_cast<int>(mass1 + mass2);
    if (numNewMolecules <= 0) return;
    
    Vector2f vel1 = reactor.molecules[i]->getVelocity();
    Vector2f vel2 = reactor.molecules[j]->getVelocity();

    Vector2f totalMomentum = vel1 * mass1 + vel2 * mass2;
    
    reactor.markForRemoval(i);
    reactor.markForRemoval(j);
    
    numNewMolecules = std::min(numNewMolecules, 50);
    
    float safeRadius = std::min(
        std::min(reactor.reactorWidth - 2 * reactor.wallThickness, reactor.reactorHeight - 2 * reactor.wallThickness) / 2 - reactor.moleculeRadius,
        numNewMolecules * reactor.moleculeRadius * 2.0f
    );
    
    float maxX = reactor.reactorX + reactor.reactorWidth  - reactor.wallThickness - reactor.moleculeRadius;
    float minX = reactor.reactorX + reactor.wallThickness + reactor.moleculeRadius;
    float maxY = reactor.reactorY + reactor.reactorHeight - reactor.wallThickness - reactor.moleculeRadius;
    float minY = reactor.reactorY + reactor.wallThickness + reactor.moleculeRadius;
    
    Vector2f safeCollisionPos = collisionPos;
    safeCollisionPos.setX(std::clamp(collisionPos.getX(), minX + safeRadius, maxX - safeRadius));
    safeCollisionPos.setY(std::clamp(collisionPos.getY(), minY + safeRadius, maxY - safeRadius));
    
    Vector2f commonVelocity = totalMomentum * (1.0f / static_cast<float>(numNewMolecules));
    
    std::vector<Vector2f> radialOffsets;
    Vector2f totalRadial(0, 0, 0);
    
    for (int k = 0; k < numNewMolecules; ++k) {
        float angle = 2 * 3.14159f * k / numNewMolecules;
        float rx = std::cos(angle);
        float ry = std::sin(angle);
        radialOffsets.emplace_back(rx, ry, 0);
        totalRadial = totalRadial + radialOffsets.back();
    }
    
    Vector2f avgRadial = totalRadial * (1.0f / static_cast<float>(numNewMolecules));
    for (auto& r : radialOffsets) {
        r = r - avgRadial;
    }
    
    float spreadScale = reactor.moleculeSpeed * 0.3f;
    
    for (int k = 0; k < numNewMolecules; ++k) {
        float angle = 2 * 3.14159f * k / numNewMolecules;
        float offsetX = std::cos(angle) * safeRadius;
        float offsetY = std::sin(angle) * safeRadius;
        
        float posX = safeCollisionPos.getX() + offsetX;
        float posY = safeCollisionPos.getY() + offsetY;
        
        posX = std::clamp(posX, minX, maxX);
        posY = std::clamp(posY, minY, maxY);
        
        Vector2f finalVel = commonVelocity + radialOffsets[k] * spreadScale;
        
        reactor.addRoundMoleculeAt(posX, posY, finalVel.getX(), finalVel.getY());
    }
}

void Reactor::updateMoleculePositions(float dt) {
    for (auto& mol : molecules) {
        if (mol) {
            mol->update(dt);
            handleWallCollisions(*mol);
        }
    }
}

void Reactor::handleWallCollisions(Molecule& mol) {
    Vector2f pos = mol.getPosition();
    Vector2f size = mol.getSize();

    if (pos.getX() - size.getX()/2 <= reactorX + wallThickness) {
        Vector2f vel = mol.getVelocity();
        mol.setVelocity(std::abs(vel.getX()) * leftWallTemperature, vel.getY() * leftWallTemperature);
        mol.setPosition(reactorX + wallThickness + size.getX()/2, pos.getY());
    }
    else if (pos.getX() + size.getX()/2 >= reactorX + reactorWidth - wallThickness) {
        Vector2f vel = mol.getVelocity();
        mol.setVelocity(-std::abs(vel.getX()), vel.getY());
        mol.setPosition(reactorX + reactorWidth - wallThickness - size.getX()/2, pos.getY());
        rightWallHitsLastSecond++;
    }
    if (pos.getY() - size.getY()/2 <= reactorY + wallThickness) {
        Vector2f vel = mol.getVelocity();
        mol.setVelocity(vel.getX(), std::abs(vel.getY()));
        mol.setPosition(pos.getX(), reactorY + wallThickness + size.getY()/2);
    }
    else if (pos.getY() + size.getY()/2 >= reactorY + reactorHeight - wallThickness) {
        Vector2f vel = mol.getVelocity();
        mol.setVelocity(vel.getX(), -std::abs(vel.getY()));
        mol.setPosition(pos.getX(), reactorY + reactorHeight - wallThickness - size.getY()/2);
    }
}

void Reactor::updateStatistics() {
    float totalEnergy = 0;
    int roundCount = 0;
    int squareCount = 0;
    
    for (const auto& mol : molecules) {
        if (mol) {
            Vector2f vel = mol->getVelocity();
            float speedSq = vel.getX() * vel.getX() + vel.getY() * vel.getY();
            totalEnergy += 0.5f * mol->getMass() * speedSq;
            
            if (mol->getType() == MoleculeType::Round) roundCount++;
            else squareCount++;
        }
    }
    
    float temperature = molecules.empty() ? 0 : totalEnergy / std::max(1, (int)molecules.size());

    moleculeHistory.      push_back(molecules.size());
    roundMoleculeHistory. push_back(roundCount);
    squareMoleculeHistory.push_back(squareCount);
    energyHistory.        push_back(totalEnergy);
    temperatureHistory.   push_back(temperature);
}