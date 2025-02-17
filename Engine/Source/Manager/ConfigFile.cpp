#include "ConfigFile.h"

#include <include/glm_utils.h>
#include <include/pugixml.h>

#include <Config/ResourcePath.h>
#include <Core/WindowObject.h>

#include <Manager/Manager.h>
#include <Manager/RenderingSystem.h>

using namespace std;

static pugi::xml_node *config;
static pugi::xml_document *doc;

ConfigFile::ConfigFile() {
}

ConfigFile::~ConfigFile() {
}

void ConfigFile::Load(const char *fileName)
{
	doc = pugi::LoadXML((RESOURCE_PATH::CONFIG + "config.xml").c_str());
	config = new pugi::xml_node(doc->child("config"));

	// Read window info
	auto window = config->child("window");
	windowProperties = new WindowProperties();
	windowProperties->name			= window.child_value("name");
	windowProperties->fullScreen	= window.child("fullscreen").text().as_bool();
	windowProperties->resolution	= glm::ExtractVector<glm::ivec2>(window.child_value("resolution"));
	windowProperties->position		= glm::ExtractVector<glm::ivec2>(window.child_value("position"));
	windowProperties->centered		= window.child("centered").text().as_bool();
	windowProperties->visible		= window.child("visible").text().as_bool();

	// Get GL context creation info
	auto node = window.child("glContext");
	windowProperties->glContext.major = node.child("major").text().as_int();
	windowProperties->glContext.minor = node.child("minor").text().as_int();
}

void ConfigFile::ReadGraphicState(const char* propertyName, RenderState STATE)
{
	const char *value = config->child_value(propertyName);
	bool state = false;
	if (strcmp(value, "true") == 0)
		state = true;

	Manager::RenderSys->Set(STATE, state);
}

string ConfigFile::GetResourceFileLoc(const char *resourceID)
{
	const char* fileName = config->child_value(resourceID);
	if (!fileName) {
		printf("<%s> entry was not specified\n", resourceID);
		assert(false);
	}

	return RESOURCE_PATH::CONFIG + fileName;
};

std::vector<string> ConfigFile::GetResourceFiles(const char *resourceID)
{
	vector<string> configFiles;
	auto files = config->child(resourceID);

	for (auto configFile : files.children())
	{
		configFiles.push_back(RESOURCE_PATH::CONFIG + configFile.child_value());
	}

	return configFiles;
};