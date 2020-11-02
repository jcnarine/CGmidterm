#include "ScoreUI.h"
#include "Utilities/ObjLoader.h"

ScoreUI::ScoreUI()
{

}

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
        vao = ObjLoader::LoadFromFile("Objects/ScoreCounter-0-FINAL.obj");
    }
}
