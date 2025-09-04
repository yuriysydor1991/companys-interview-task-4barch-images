import QtQuick
import QtQuick.Controls 2.12

ListView {
  id: compRoot
  
  anchors.fill: parent
  
  readonly property color colorSelected: "#0a0"
  readonly property color colorRegular: "#fff"
  readonly property color colorBorder: "#777"
  
  property int selectedIndex: -1

  model: ImagesFilesListProvider
  
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
      
      text: path + " [" + size + "]" + (current_operation != "" ? " [" +current_operation + "]" : "" )
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
