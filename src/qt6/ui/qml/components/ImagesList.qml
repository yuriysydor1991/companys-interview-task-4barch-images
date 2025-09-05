import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Layouts

Item {
  readonly property color colorBorder: "#777"
  
  anchors.fill: parent
    
  ColumnLayout {
    anchors.fill: parent
    
    Text {
      id: windgetTitle
      
      Layout.fillWidth: true
      
      font.pixelSize: 14
      font.bold: true
      text: "Список файлів для перетворення. Для перетворення необхідно клікнути по рядку файлу."
    }
    
    ListView {
      id: listView
      
      Layout.fillWidth: true
      Layout.fillHeight: true
      
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
            listView.model.convert_file(index)
            listView.selectedIndex = index
          }
        }
      }
    } // listview
  } // column
} // item
