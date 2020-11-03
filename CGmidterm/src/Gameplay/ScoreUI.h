#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include "Graphics/VertexArrayObject.h"
class ScoreUI
{
public:
	
	ScoreUI();
	~ScoreUI();

	//Getter's and setters to Grab VAO's
	void setCurrentVAO(VertexArrayObject::sptr& v);
	VertexArrayObject::sptr getCurrentVAO();

	void SetPlayerScore();

	void ScoreUI::setTotalScore(int total);

	int ScoreUI::getTotalScore();
	


private:

	//Shared pointer of VAO class is created called 'vao'.
	VertexArrayObject::sptr vao;

	int totalScore = 0;

};

