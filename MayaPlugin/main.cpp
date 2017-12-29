#include "maya_includes.h"

MCallbackIdArray myCallbackArray;

void renderChangeCallback(const MString &str, void *clientData)
{
	M3dView sceneView;
	sceneView = sceneView.active3dView();
	int numViews = sceneView.numberOf3dViews();
	MGlobal::displayInfo(MString("Views:") + numViews);
	MDagPath camDag;
	sceneView.getCamera(camDag);

	MCallbackId CameraChangedId = MNodeMessage::addAttributeChangedCallback(
		camDag.node(),
		CameraChangedCallback,
		)

	if (res == MS::kSuccess)
	{
		if (myCallbackArray.append(nodeAddedId) == MS::kSuccess);
	}


	if (camDag.node().apiType() == MFn::kCamera)
	{
		MFnCamera cam = camDag.node();
		MFnTransform camTransform = cam.parent(0);
		MMatrix mat = camTransform.transformation().asMatrix().matrix;

		MGlobal::displayInfo(MString("Cam:") + mat.matrix[0][0] + " " + mat.matrix[0][1] + " " + mat.matrix[0][2] + " " + mat.matrix[0][3]);
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[1][0] + " " + mat.matrix[1][1] + " " + mat.matrix[1][2] + " " + mat.matrix[1][3]);
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[2][0] + " " + mat.matrix[2][1] + " " + mat.matrix[2][2] + " " + mat.matrix[2][3]);
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[3][0] + " " + mat.matrix[3][1] + " " + mat.matrix[3][2] + " " + mat.matrix[3][3]);

		double ARO = cam.aspectRatio();
		double FOV = cam.horizontalFieldOfView();
		double FOV2 = cam.verticalFieldOfView();
		double nPlane = cam.nearClippingPlane();
		double fPlane = cam.farClippingPlane();

		MGlobal::displayInfo(MString("Cam:") + ARO);
		MGlobal::displayInfo(MString("Cam:") + FOV);
		MGlobal::displayInfo(MString("Cam:") + FOV2);
		MGlobal::displayInfo(MString("Cam:") + nPlane);
		MGlobal::displayInfo(MString("Cam:") + fPlane);
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

void CameraChangedCallback()
{

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