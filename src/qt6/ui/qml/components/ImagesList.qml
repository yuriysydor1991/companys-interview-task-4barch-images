import QtQuick
import QtQuick.Controls 2.12

ListView {
  id: compRoot
  
  anchors.fill: parent
  
  readonly property color colorSelected: "#0a0"
  readonly property color colorRegular: "#fff"
  readonly property color colorBorder: "#444"
  
  property int selectedIndex: -1

  model: [
    { "path": "/tmp/nofile1.barch", "size": 123, "selected": false },
    { "path": "/tmp/nofile2.barch", "size": 456, "selected": false },
    { "path": "/tmp/nofile2.barch", "size": 789, "selected": false },
  ]
  
  delegate: Rectangle {
    id: delRoot
    
    width: parent.width
    height: fileDisplay.height
    
    color: getBG()
    
    border.color: colorBorder
    
    Text {
      id: fileDisplay
      anchors.top: parent.top
      anchors.left: parent.left
      
      text: modelData.path + " [" + modelData.size + "]"
    }
    
    MouseArea {
      anchors.fill: parent
      onClicked: { 
        console.log("Row clicked: " + modelData.path)
        modelData.selected = true
        compRoot.selectedIndex = index
        delRoot.updateBG()
      }
    }
    
    function updateBG() {
      color = getBG()
    }
    
    function getBG() {
      console.log("row is selected" + modelData.selected)
      return compRoot.selectedIndex == index ? colorSelected : colorRegular
    }
  }
}
