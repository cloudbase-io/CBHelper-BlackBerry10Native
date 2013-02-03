// Tabbed Pane project template
import bb.cascades 1.0
import CustomComponent 1.0

TabbedPane {
    showTabsOnActionBar: true
    Tab {
        title: qsTr("Settings")
        Page {
            id: tab1
            actions: [
                // define the actions for first tab here
                ActionItem {
                    title: qsTr("Rotate")
                    onTriggered: {
                        imgTab1.rotationZ = imgTab1.rotationZ + 90;
                    }
                },
                ActionItem {
                    title: qsTr("Break")
                    onTriggered: {
                        imgTab1.imageSource = "asset:///images/picture1br.png";
                    }
                }
            ]
            Container {
                // define tab content here
                Label {
                    text: qsTr("cloudbase.io configuration")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                TextField {
                    id: appCodeField
                    hintText: "Application code"
                    topMargin: 30.0
                    text: "mosync-test"
                }
                TextField {
                    id: appSecretField
                    hintText: "Application unique code"
                    text: "d2b4e02ab1e415babb45ffa379da25ea"
                }
                TextField {
                    id: appPwdField
                    hintText: "Application password"
                    inputMode: TextFieldInputMode.Password
                    text: "m1i2n3e4"
                }
                Button {
                    id: saveConfigButton
                    text: "Save"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        console.log("event from qml");
                        
                        app.saveButtonClicked(appCodeField.text, appSecretField.text, appPwdField.text);
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("Log")
        Page {
            id: tab2
            actions: [
                // define the actions for tab here
                ActionItem {
                    title: qsTr("Raise")
                    onTriggered: {
                        // run the image animation
                        raiseAnimation.play();
                    }
                }
            ]
            Container {
                // define tab content here
                Label {
                    text: qsTr("cloudbase.io log APIs")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                TextField {
                    id: logTextField
                    hintText: "Log message"
                    topMargin: 30.0
                }
                Button {
                    id: sendLogButton
                    text: "Log Debug Message"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.logButtonClicked(logTextField.text);
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("Data")
        Page {
            id: tab3
            Container {
                // define tab content here
                Label {
                    text: qsTr("cloudbase.io data APIs")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                Button {
                    id: insertDataButton
                    text: "Insert test object"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.insertButtonClicked();
                    }
                }
                Button {
                    id: insertFileDataButton
                    text: "Insert test object with file"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        filePicker.open()
                    }
                }
                Button {
                    id: searchDataButton
                    text: "Search data"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.searchButtonClicked();
                    }
                }
                Button {
                    id: searchDataAggregateButton
                    text: "Search Aggregate data"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.searchAggregateButtonClicked();
                    }
                }
                Button {
                    id: downloadFileButton
                    text: "Download File"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.downloadFileButtonClicked("1c7af457b2e6150173717f1ebf530834");
                    }
                }
            }
            attachedObjects: [
                FilePicker {
                    id:filePicker
                    type : FileType.Picture
                    title : "Select Picture"
                    directories : ["/"]
                    onFileSelected : {
                        app.insertFileButtonClicked(selectedFiles);
                    }
                }
            ]
        }
    }
    Tab {
        title: qsTr("CloudFunctions")
        Page {
            id: tab4
            actions: [
                // define the actions for tab here
                ActionItem {
                    title: qsTr("Raise")
                    onTriggered: {
                        // run the image animation
                        raiseAnimation.play();
                    }
                }
            ]
            Container {
                // define tab content here
                Label {
                    text: qsTr("cloudbase.io CloudFunction APIs")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                TextField {
                    id: functionCodeTextField
                    hintText: "CloudFunction Code"
                    topMargin: 30.0
                }
                Button {
                    id: execFunctionButton
                    text: "Execute CloudFunction"
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked: {
                        app.execFunctionButtonClicked(functionCodeTextField.text);
                    }
                }
                Button {
                    id: execAppletButton
                    text: "Execute Applet"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.execAppletButtonClicked();
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("PayPal")
        Page {
            id: tab5
            actions: [
                // define the actions for tab here
                ActionItem {
                    title: qsTr("Raise")
                    onTriggered: {
                        // run the image animation
                        raiseAnimation.play();
                    }
                }
            ]
            Container {
                // define tab content here
                Label {
                    text: qsTr("cloudbase.io PayPal APIs")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                Button {
                    id: startPayPalButton
                    text: "Log Debug Message"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 30.0
                    onClicked: {
                        app.payPalButtonClicked();
                    }
                }
            }
        }
        }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("TabbedPane - onCreationCompleted()")

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
