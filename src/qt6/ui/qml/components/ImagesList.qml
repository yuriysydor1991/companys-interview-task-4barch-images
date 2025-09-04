import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Layouts

Item {
  readonly property color colorBorder: "#777"
  
  anchors.fill: parent
    
  Text {
    id: windgetTitle
    
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    
    font.pixelSize: 14
    font.bold: true
    text: "Список файлів для перетворення. Для перетворення необхідно клікнути по рядку файлу."
  }
  
  ListView {
    id: listView
    
    anchors.top: windgetTitle.bottom
    anchors.left: parent.left
    
    width: parent.width
    height: 500
    
    property int selectedIndex: -1

    model: ImagesFilesListProvider
    clip: true
    
    delegate: Rectangle {
      id: delRoot
      
      width: parent.width
      height: fileDisplay.height
      
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
          console.log("Row clicked: " + path)
          listView.model.convert_file(index)
          listView.selectedIndex = index
        }
      }
    }
  } // listview
} // item
