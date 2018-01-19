#include "maya_includes.h"

MCallbackIdArray myCallbackArray;

void renderChangeCallback(const MString &str, void *clientData)
{
	M3dView sceneView;
	sceneView = sceneView.active3dView();
	MDagPath camDag;
	sceneView.getCamera(camDag);

	int numViews = sceneView.numberOf3dViews();
	MGlobal::displayInfo(MString("Views:") + numViews);
	MDagPath camDag;
	sceneView.getCamera(camDag);

	/*MCallbackId CameraChangedId = MNodeMessage::addAttributeChangedCallback(
		camDag.node(),
		CameraChangedCallback,
		);*/

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


//MESH INFO//

//VERTISER//
void OutputMeshVerticis(MObject& obj)
{
	MString displayLenght;
	MString dipslayVert;
	MFnMesh fn(obj);
	MPointArray vts;
	fn.getPoints(vts);

	displayLenght += "Number of vertisis: ";
	displayLenght += vts.length();

	MGlobal::displayInfo(displayLenght);

	for (int i = 0; i != vts.length(); ++i)
	{
		dipslayVert += "X: ";
		dipslayVert += vts[i].x;
		dipslayVert += "Y: ";
		dipslayVert += vts[i].x;
		dipslayVert += "Z: ";
		dipslayVert += vts[i].x;

	}

	MGlobal::displayInfo(dipslayVert);
}

//NORMALER//
void outputMeshNormals(MObject& obj)
{
	MString displayLenght;
	MString displayNorm;
	MFnMesh fn(obj);
	MFloatVectorArray nmls;

	fn.getNormals(nmls);

	displayLenght += "Number of normals: ";
	displayLenght += nmls.length();
	MGlobal::displayInfo(displayLenght);

	for (int i = 0; i != nmls.length(); ++i)
	{
		displayNorm += "X: ";
		displayNorm += nmls[i].x;
		displayNorm += "Y: ";
		displayNorm += nmls[i].x;
		displayNorm += "Z: ";
		displayNorm += nmls[i].x;

	}

	MGlobal::displayInfo(displayNorm);
}


//UV
bool OutputMeshTextureCoords(MObject& obj)
{
	bool uv = true;
	MString displayLenght;
	MString displayUV;
	MFnMesh fn(obj);
	MStringArray uvsets;
	fn.getUVSetNames(uvsets);

	if (!uvsets.length() || !fn.numUVs(uvsets[0]))
	{
		MGlobal::displayInfo("no uvs");
		uv = false;
	}
	else
	{
		displayLenght += ("lenght of UV coordinates");
		displayLenght += uvsets.length();

		for (int i = 0; i != uvsets.length(); ++i)
		{
			//to se the name
			displayUV += ("Uv Name");
			displayUV += uvsets[i];

			MFloatArray UCoords;
			MFloatArray VCoords;

			fn.getUVs(UCoords, VCoords, &uvsets[i]);

			displayUV += "number of UV";
			displayUV += fn.numUVs(uvsets[i]);

			for (int j = 0; j != fn.numUVs(uvsets[i]); ++j)
			{
				int numUvs = fn.numUVs(uvsets[j]);
				float uC = UCoords[j];
				float vC = VCoords[j];
				displayUV += UCoords[j];
				displayUV += " ";
				displayUV += VCoords[j];
				displayUV += " ";
			}
		}

		MGlobal::displayInfo(displayUV);
	}

	return uv;
}


//TRANSFORMATIONER
void getTransfromData(MObject& object)
{
	MFnTransform obj(object);

	MMatrix matrix = obj.transformation().asMatrix();
	MString DispScale;
	MString DipsTrans;
	MString DipsRot;

	//scale
	double scale[3] = { 1, 1, 1 };
	obj.getScale(scale);

	DispScale += "Scale: ";
	DispScale += "X: ";
	DispScale += scale[0];
	DispScale += "Y: ";
	DispScale += scale[1];
	DispScale += "Z: ";
	DispScale += scale[2];

	//Translation 
	MVector Translation = obj.getTranslation(MSpace::kObject);

	DipsTrans += "Translation: ";
	DipsTrans += "X: ";
	DipsTrans += Translation.x;
	DipsTrans += "Y: ";
	DipsTrans += Translation.y;
	DipsTrans += "Z: ";
	DipsTrans += Translation.z;

	//Rotation 
	MQuaternion Rotation(0, 0, 0, 1);
	obj.getRotation(Rotation);
	DipsRot += "Rotation: ";
	DipsRot += "X: ";
	DipsRot += Rotation.x;
	DipsRot += "Y: ";
	DipsRot += Rotation.y;
	DipsRot += "Z: ";
	DipsRot += Rotation.z;
	DipsRot += "W: ";
	DipsRot += Rotation.w;

	MGlobal::displayInfo(DispScale);
	MGlobal::displayInfo(DipsTrans);
	MGlobal::displayInfo(DipsRot);


}

//MESH NAME
void MeshName(MObject& object)
{

	//går igenom alla meshes i en sen
	MItDag it(MItDag::kDepthFirst, MFn::kMesh);
	while (!it.isDone())
	{
		MFnMesh obj(it.item());

		MObject TextureObj(object);

		if (!obj.isIntermediateObject())
		{
			MString display;
			display += "Mesh Name: ";
			display += obj.name();
			MGlobal::displayInfo(display);
			getTransfromData(object);

			//FIND TEXTURES
			//if (TextureObj.hasFn(MFn::kFileTexture))
			//{
			//	MString TextureFileName = MFn::kFileTexture; 
			//	//spara texture path
			//}
		}

		it.next();
	}

}

void TexturePath()
{
	MItDependencyNodes iterator(MFn::kFileTexture); 

	MObjectArray filenames; 

	while (!iterator.isDone())
	{
		MFnDependencyNode fn(iterator.item()); 

		filenames.append(iterator.item()); 
		MPlug FullPath = fn.findPlug("ftn"); 
		MString texturePath; 
		FullPath.getValue(texturePath); 
		iterator.next(); 

		MGlobal::displayInfo(texturePath); 
	}
}


//NODE RENAME
void NodeRename(MObject &node, const MString &LastName, void* clientData)
{
	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh myMesh(node);

		if (myMesh.name() != LastName)
		{
			/*MString msg("Node Renamed");
			MGlobal::displayInfo(msg);*/

			MString name = myMesh.name();
			MGlobal::displayInfo("Name changed: " + name);
		}
	}
}


//When somthing changes
void TopologyChanged(MObject &node, void* clientData)
{
	MString msg("Topology changed!");
	MGlobal::displayInfo(msg);
}


//Material//
void ChangedAttribute(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	MStatus res;
	MObject myMesh(plug.node());

	if (msg & MNodeMessage::AttributeMessage::kAttributeSet)
	{
		MString displayVertex;

		MPoint p[8];
		double VertArr[4];
	
		if (plug.node().apiType() == MFn::kMesh)
		{
			MGlobal::displayInfo("Mesh Attribute Changed");
			OutputMeshVerticis(myMesh);
			outputMeshNormals(myMesh);
			OutputMeshTextureCoords(myMesh);
			MeshName(myMesh);
			TexturePath();
		}
		
	}

		//MGlobal::displayInfo(plug.node().apiTypeStr());

		if (plug.node().apiType() == MFn::Type::kTransform)
		{
			MGlobal::displayInfo("Tranform Attribute Changed");
			OutputMeshVerticis(myMesh);
			MeshName(myMesh);
		}

		if (plug.node().hasFn(MFn::kBlinn))
		{
			MGlobal::displayInfo("Blinn Material");
			MGlobal::displayInfo(plug.name());
			MFnBlinnShader ColorValues = plug.node();
			MColor color;
			color = ColorValues.color();
			MString displayColor;
			displayColor += color.r;
			displayColor += " ";
			displayColor += color.g;
			displayColor += " ";
			displayColor += color.b;

			MGlobal::displayInfo(displayColor);

		}
		else if (plug.node().hasFn(MFn::kLambert))
		{
			MGlobal::displayInfo("Lambert Material");
			MGlobal::displayInfo(plug.name());
			MFnLambertShader ColorValues = plug.node();
			MColor color;
			color = ColorValues.color();
			MString displayColor;

			displayColor += color.r;
			displayColor += " ";
			displayColor += color.g;
			displayColor += " ";
			displayColor += color.b;
		

<<<<<<< HEAD
		}
	
=======
		
	}
>>>>>>> master
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

	// de två sista i parameterna är valbar infomation 
	MObject MObject;

	MCallbackId idChanged = MNodeMessage::addAttributeChangedCallback(MObject, ChangedAttribute, NULL, &res);
	if (res == MS::kSuccess)
	{
		if (myCallbackArray.append(idChanged) == MS::kSuccess);
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