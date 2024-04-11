#include "Object.h"
#include "Edge.h"
#include "Face.h"

class CatmullClark {
    private:
        Object& object;
        int level;
        vector<Edge*> edges;
        vector<Face> catmullFaces;
        vector<glm::vec3> edgePointsCoord;
        vector<glm::vec3> facePointsCoord;
        vector<glm::vec3> newVertexPointsCoord;
        vector<pair<int, int>> newEdgesForWireframe;
        int baseLevel;


    public:
        //CatmullClark(glm::vec3 translation, const string& fileName, glm::vec3 color, const string& vertexShader, const string& fragmentShader, glm::vec3 rotation);
        CatmullClark(Object& obj, int baseLevel);

        ~CatmullClark();

        void setMatrices(double deltaTime);

        void setProjectionMatrix(glm::mat4 proj);

        void setViewingMatrix(glm::mat4 view);

        void drawObject();

        void increaseLevel();

        void decreaseLevel();

        void reset();

        void start_pause();
        
        void initFaceAndEdgePoints();
        std::unordered_map<Edge, std::vector<int>, EdgeHash> findFacesForEachEdge(std::vector<Face>& faces);
        void changeMode();
        void setDepthBuffer();

       

};