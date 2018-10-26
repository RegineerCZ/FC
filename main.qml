import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.11

Window{
    visible: true
    width: 640
    height: 480
    title: qsTr("Factorio Clone")

    /*Component.onCompleted: {
        img.source = "image://renderProvider/test"
    }*/

    Image{
        id: img
        width: 640
        height: 480
        source: "image://renderProvider/test"
        fillMode: Image.PreserveAspectCrop
        visible: true
        property int i: 0

        function updateSource(){
            if (i === 0){
                img.source = "image://renderProvider/test2"
                i = 1
            } else {
                img.source = "image://renderProvider/test"
                i = 0
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onMouseXChanged: {
            img.updateSource()
        }
        onMouseYChanged: {
            img.updateSource()
        }
        onClicked: {

        }
    }
}
