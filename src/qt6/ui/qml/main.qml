import QtQuick
import QtQuick.Controls 2.12

import "./components"

ApplicationWindow {
  id: mainWindow
  
  visible: true
  
  property var errorModel: ErrorProvider
  property var errMsg: errorModel.error_description

  width: 800
  height: 300
  minimumWidth: 400
  minimumHeight: 250

  ImagesList {}
  
  Loader {
    id: errorDialogLoader
    source: "qrc:/ui/qml/components/ErrorDialog.qml"
    active: !get_e_empty()
    
    onLoaded: update_error()
  }
  
  onErrMsgChanged: update_error()
  
  function update_error() {
    if (get_e_empty() || !errorDialogLoader.item) {
      return
    }
    
    errorDialogLoader.item.text = errMsg
    errorDialogLoader.item.open()
  }
  
  function get_e_empty() {
    return typeof errMsg === 'undefined' || errMsg.length == 0;
  }
}

