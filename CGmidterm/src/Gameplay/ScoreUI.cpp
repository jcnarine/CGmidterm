#include "ScoreUI.h"
#include "Utilities/ObjLoader.h"

//Constructor
ScoreUI::ScoreUI()
{

}
//Destructor
ScoreUI::~ScoreUI()
{

}

void ScoreUI::setCurrentVAO(VertexArrayObject::sptr& v)
{
    //Set vao variable to current Vao that's accesed.
    vao = v;

}

VertexArrayObject::sptr ScoreUI::getCurrentVAO()
{
    return vao;
}

//Void function that Changes the OBJ and sets the current score
void ScoreUI::SetPlayerScore()
{
    if (totalScore == 1)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-1.obj");
    }
    if (totalScore == 2)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-2.obj");
    }
    if (totalScore == 3)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-3.obj");
    }
    if (totalScore == 4)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-4.obj");
    }
    if (totalScore == 5)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-5.obj");
    }
    if (totalScore == 6)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-6.obj");
    }
    if (totalScore == 7)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-7.obj");
    }
    if (totalScore == 8)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-8.obj");
    }
    if (totalScore == 9)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-9.obj");
    }
    if (totalScore == 10)
    {
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-10.obj");
    }
}

void ScoreUI::setTotalScore(int total)
{
    totalScore = total;
}

int ScoreUI::getTotalScore()
{
    return totalScore;
}
