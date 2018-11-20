import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

Window{
    visible: true
    width: 300
    height: 300
    title: qsTr("FC")

    Image{
        id: img
        //width: 300
        //height: 300
        anchors.fill: parent
        source: ""//"image://renderProvider/test"
        fillMode: Image.Pad
        visible: true
        cache: false
        property int i: 0

        function updateSource(){
            console.log("update source: ", i)
            if (i === 0){
                img.source = ""
                img.update()
                img.source = "image://renderProvider/test2"
                img.update()
                i = 1
            } else {
                img.source = ""
                img.update()
                img.source = "image://renderProvider/test"
                img.update()
                i = 0
            }
        }

    }

    /*Timer{
        interval: 200; repeat: true; running:true
        onTriggered: {
            img.updateSource()
        }
    }*/


    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onMouseXChanged: {
            //console.log("mousechangeX")
            com.mousePosChanged(mouseX, mouseY)
            img.updateSource()
        }
        onMouseYChanged: {
            //console.log("mousechangeY")
            com.mousePosChanged(mouseX, mouseY)
            img.updateSource()
        }
        onClicked: {

        }


        focus: true
        Keys.onPressed: {
            console.log("Key Pressed: ", event.key)

            if (event.key === Qt.Key_R) {
                com.regenerateMap();
            }
            if (event.key === Qt.Key_G) {
                img.updateSource()
            }
        }
    }
}
