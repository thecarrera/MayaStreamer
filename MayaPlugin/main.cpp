#include "maya_includes.h"

MCallbackIdArray myCallbackArray;

void renderChangeCallback(const MString &str, void *clientData)
{
	M3dView sceneView;
	sceneView = sceneView.active3dView();
	int numViews = sceneView.numberOf3dViews();
	MGlobal::displayInfo(MString() + numViews);
	MDagPath camDag;
	sceneView.getCamera(camDag);



	if (camDag.node().apiType() == MFn::kCamera)
	{
		MFnCamera cam = camDag.node();
		MVector forwardVec;
		MVector upVec;
		MVector rightVec;

		forwardVec = cam.viewDirection(MSpace::kObject);
		upVec = cam.upDirection(MSpace::kObject);
		rightVec = cam.rightDirection(MSpace::kObject);
		cam.

			MGlobal::displayInfo("forward");
		MGlobal::displayInfo(MString() + forwardVec.x);
		MGlobal::displayInfo(MString() + forwardVec.y);
		MGlobal::displayInfo(MString() + forwardVec.z);

		MGlobal::displayInfo("up");
		MGlobal::displayInfo(MString() + upVec.x);
		MGlobal::displayInfo(MString() + upVec.y);
		MGlobal::displayInfo(MString() + upVec.z);

		MGlobal::displayInfo("Right");
		MGlobal::displayInfo(MString() + rightVec.x);
		MGlobal::displayInfo(MString() + rightVec.y);
		MGlobal::displayInfo(MString() + rightVec.z);
	}
}

void nameChangeCallback(MObject& node, const MString &string, void* clientData)
{
	MFnMesh mesh(node);
	MString msg(mesh.className());
	MGlobal::displayInfo(msg + "tatata");
}

//##########################################################################
void nodeCreationCallback(MObject& node, void* clientData)
{
	MStatus res;

	if (node.apiType() == MFn::kMesh)
	{
		MCallbackId namechangeid = MNodeMessage::addNameChangedCallback(
			node,
			nameChangeCallback,
			NULL,
			&res);

		if (res == MS::kSuccess)
		{
			if (myCallbackArray.append(namechangeid) == MS::kSuccess);
		}


	}

}

EXPORT MStatus initializePlugin(MObject obj) {
	// most functions will use this variable to indicate for errors
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	// if res == kSuccess then the plugin has been loaded,
	// otherwise is has not.
	if (MFAIL(res)) {
		CHECK_MSTATUS(res);
		return res;
	}
	else {
		MGlobal::displayInfo("Maya plugin loaded!");
	}

	MCallbackId nodeAddedId = MDGMessage::addNodeAddedCallback(
		nodeCreationCallback,
		kDefaultNodeType,
		NULL,
		&res);

	if (res == MS::kSuccess)
	{
		if (myCallbackArray.append(nodeAddedId) == MS::kSuccess);
	}

	MCallbackId camTranslateId = MUiMessage::add3dViewPostRenderMsgCallback(
		"modelPanel4",
		renderChangeCallback,
		NULL,
		&res);

	if (res == MS::kSuccess)
	{
		if (myCallbackArray.append(camTranslateId) == MS::kSuccess);
	}

	return res;
}

EXPORT MStatus uninitializePlugin(MObject obj) {
	// simply initialize the Function set with the MObject that represents
	// our plugin
	MFnPlugin plugin(obj);

	// if any resources have been allocated, release and free here before
	// returning...
	MMessage::removeCallbacks(myCallbackArray);
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}