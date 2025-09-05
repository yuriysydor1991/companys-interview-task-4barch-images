import QtQuick
import QtQuick.Controls 2.12

Dialog {
  id: root

  property var text: ""
  
  modal: true  
  title: "Error"
  
  standardButtons: Dialog.Ok
  
  contentItem: Text {
    text: root.text
  }
}
