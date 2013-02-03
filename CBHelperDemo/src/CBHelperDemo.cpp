// Tabbed pane project template
#include "CBHelperDemo.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <QSettings>

#include "CBPayPal.h"

using namespace bb::cascades;

CBHelperDemo::CBHelperDemo(bb::cascades::Application *app)
: QObject(app)
{
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    QSettings settings;
    if ( !settings.value("appCode").isNull() ) {
    	// TODO: set value in fields here.
    }
    // create root object for the UI
    qml->setContextProperty("app", this);
    root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene

    app->setScene(root);
}

Q_INVOKABLE void CBHelperDemo::saveButtonClicked(QString appCode, QString appSecret, QString appPwd) {
	this->helper = new Cloudbase::CBHelper(appCode.toStdString(), appSecret.toStdString());
	QString password = QString(QCryptographicHash::hash(appPwd.toUtf8(), QCryptographicHash::Md5).toHex());
	this->helper->setPassword(password.toStdString());

	QSettings settings;
	settings.setValue("appCode", appCode);
	settings.setValue("appSecret", appSecret);
	settings.setValue("appPwd", password);
}

Q_INVOKABLE void CBHelperDemo::logButtonClicked(QString logMessage) {
	this->helper->logDebug(logMessage.toStdString(), "DEFAULT");
}

Q_INVOKABLE void CBHelperDemo::insertButtonClicked() {
	doc = new TestData("Cloud", "Base.io", "BB10");
	helper->insertDocument("users_bb10", dynamic_cast<Cloudbase::CBSerializable*>(doc), NULL);
}

Q_INVOKABLE void CBHelperDemo::insertFileButtonClicked(QString filePath) {
	doc = new TestData("Cloud", "Base.io", "BB10");

	att.fileName = "testfile.jpg";
	att.filePath = filePath.toStdString();

	atts.insert(atts.begin(), att);

	helper->insertDocument("users_bb10", dynamic_cast<Cloudbase::CBSerializable*>(doc), atts, NULL);
}
Q_INVOKABLE void CBHelperDemo::searchButtonClicked() {
	cond = new Cloudbase::CBHelperSearchCondition("first_name", "Cloud", Cloudbase::CBOperatorEqual);

	helper->searchDocument("users_bb10", cond, (Cloudbase::CBHelperResponder*)this);
}
Q_INVOKABLE void CBHelperDemo::searchAggregateButtonClicked() {
	commandList = new Cloudbase::CBHelperDataCommandList();

	project = new Cloudbase::CBDataAggregationCommandProject();
	project->includeFields.push_back("Symbol");
	project->includeFields.push_back("Price");
	project->includeFields.push_back("total");
	commandList->addCommand(project);

	cond = new Cloudbase::CBHelperSearchCondition("Symbol", "AAPL", Cloudbase::CBOperatorEqual);
	commandList->addCommand((Cloudbase::CBDataAggregationCommand*)cond);

	group = new Cloudbase::CBDataAggregationCommandGroup();
	group->addOutputField("Symbol");
	group->addGroupFormulaForField("total", Cloudbase::CBDataAggregationGroupSum, "Price");
	commandList->addCommand(group);

	helper->searchDocumentAggregate("security_master_3", commandList, this);
}
Q_INVOKABLE void CBHelperDemo::downloadFileButtonClicked(QString fileId) {
	helper->downloadFile(fileId.toStdString(), this);
}
Q_INVOKABLE void CBHelperDemo::execFunctionButtonClicked(QString functionCode) {
	helper->executeCloudFunction(functionCode.toStdString(), std::map<std::string, std::string>(), this);
}
Q_INVOKABLE void CBHelperDemo::execAppletButtonClicked() {
	std::map<std::string, std::string> params;
	params.insert(std::make_pair("search", "#blackberry"));

	helper->executeApplet("cb_twitter_search", params, this);
}
Q_INVOKABLE void CBHelperDemo::payPalButtonClicked() {
	Cloudbase::CBPayPalBill newBill;
	Cloudbase::CBPayPalBillItem newItem;
	newItem.amount = 9.99f;
	newItem.tax = 0.0f;
	newItem.name = "Item purchase for $9.99";
	newItem.description = "Item description for $9.99";
	newItem.quantity = 1;

	newBill.name = "Test purchase";
	newBill.description = "Test purchase for $9.99";
	newBill.invoiceNumber = "TST_INVOICE_1";
	newBill.addItem(newItem);

	helper->preparePayPalPurchase(newBill, true, this);
}

void CBHelperDemo::parseResponse(Cloudbase::CBHelperResponseInfo resp) {
	if (resp.function == "download") {
		Image image = Image(QUrl("file:///" + QString::fromStdString(resp.downloadedFileName)));
		ImageView *mImageView = new ImageView();
		mImageView->setImage(image);
		Page *imgPage = new Page();
		imgPage->setContent(mImageView);
		Application::instance()->setScene(imgPage);
	} else if (resp.function == "paypal") {
		if (resp.parsedMessage->getValueForKey("status")->isNull()) {
			QString checkoutUrl = QString::fromStdString(resp.parsedMessage->getValueForKey("checkout_url")->toString());
			payPalWebView = new WebView();
			QObject::connect(payPalWebView, SIGNAL(navigationRequest(bb::cascades::WebNavigationRequest*)), this, SLOT(monitorPayPalTransaction(bb::cascades::WebNavigationRequest*)));
			payPalWebView->setUrl(QUrl(checkoutUrl));
			Page *ppPage = new Page();
			ppPage->setContent(payPalWebView);
			Application::instance()->setScene(ppPage);
		} else {
			Application::instance()->setScene(root);
		}
	} else {
		qDebug("received response: %s", resp.outputString.c_str());
	}
}

void CBHelperDemo::monitorPayPalTransaction(bb::cascades::WebNavigationRequest *request) {
	if (request->url().toString().indexOf("http://api.cloudbase.io") != -1) {
		request->ignore();
		helper->completePayPalPurchase(request->url().toString().toStdString(), this);
	}
}
