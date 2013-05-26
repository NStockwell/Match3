
#include "GemManager.h"


int GemManager::addGem(int type, char* filename)
{
	for(int i = 0; i < this->mGems.size(); i++)
	{
		if(this->mGems[i].getType() == type)
			return -1;
	}
	this->mGems.push_back(Gem(type, filename));
}

Gem* GemManager::getGem(int type)
{
	for(int i = 0; i < mGems.size(); i++)
	{
		if(this->mGems[i].getType() == type)
		{
			Gem* newGem = new Gem(this->mGems[i].getType(), (char*)this->mGems[i].getImageFilename().c_str());
			return newGem;
		}
	}
	return NULL;
}

Gem* GemManager::getRandomTypeGem()
{
	return getGem( rand()%this->mGems.size() );
}


void GemManager::changeGemToRandomType(Gem* g)
{
	changeGemType(g, rand()%this->mGems.size() );
}

void GemManager::changeGemType(Gem* g, int type)
{
	printf("creating gem with type %d for Gem at position %f %f\n",type, g->getPosition().getX(),g->getPosition().getY());
	g->setType(type);
	for(int i = 0; i < mGems.size(); i++)
	{
		if(this->mGems[i].getType() == type)
		{
			g->setImage((char*)this->mGems[i].getImageFilename().c_str());
		}
	}
}

