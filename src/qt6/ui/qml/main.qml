import QtQuick
import QtQuick.Controls 2.12

import "./components"

ApplicationWindow {
  visible: true
  
  property var errorModel: ErrorProvider
  property var errMsg: errorModel.error_description

  width: 800
  height: 300
  minimumWidth: 400
  minimumHeight: 250

  ImagesList {}
  
  ErrorDialog {
    id: errorDialog
    
    anchors.centerIn: parent
  }
  
  onErrMsgChanged: {
    if (typeof errMsg === 'undefined' || errMsg.length == 0) {
      return
    }
    
    errorDialog.text = errMsg
    errorDialog.open()
  }
}

