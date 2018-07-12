/******************************************************************************
************************* IMPORTANT NOTE -- READ ME!!! ************************
*******************************************************************************
* THIS SOFTWARE IMPLEMENTS A **DRAFT** STANDARD, BSR E1.33 REV. 63. UNDER NO
* CIRCUMSTANCES SHOULD THIS SOFTWARE BE USED FOR ANY PRODUCT AVAILABLE FOR
* GENERAL SALE TO THE PUBLIC. DUE TO THE INEVITABLE CHANGE OF DRAFT PROTOCOL
* VALUES AND BEHAVIORAL REQUIREMENTS, PRODUCTS USING THIS SOFTWARE WILL **NOT**
* BE INTEROPERABLE WITH PRODUCTS IMPLEMENTING THE FINAL RATIFIED STANDARD.
*******************************************************************************
* Copyright 2018 ETC Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************
* This file is a part of RDMnet. For more information, go to:
* https://github.com/ETCLabs/RDMnet
******************************************************************************/

#include "RDMnetControllerGUI.h"

#include "RDMnetNetworkModel.h"
#include "NetworkDetailsProxyModel.h"
#include "SimpleNetworkProxyModel.h"
#include "PropertyEditorsDelegate.h"

RDMnetControllerGUI::~RDMnetControllerGUI()
{
  if (main_network_model_ != NULL)
  {
    delete main_network_model_;
  }

  if (simple_net_proxy_ != NULL)
  {
    delete simple_net_proxy_;
  }

  if (net_details_proxy_ != NULL)
  {
    delete net_details_proxy_;
  }
}

void RDMnetControllerGUI::handleAddBrokerByIP(std::string scope, const LwpaSockaddr &addr)
{
  emit addBrokerByIPActivated(scope, addr);
}

RDMnetControllerGUI *RDMnetControllerGUI::makeRDMnetControllerGUI()
{
  RDMnetControllerGUI *gui = new RDMnetControllerGUI;
  QHeaderView *networkTreeHeaderView = NULL;

  gui->main_network_model_ = RDMnetNetworkModel::makeRDMnetNetworkModel();  // makeTestModel();
  gui->simple_net_proxy_ = new SimpleNetworkProxyModel;
  gui->net_details_proxy_ = new NetworkDetailsProxyModel;

  gui->simple_net_proxy_->setSourceModel(gui->main_network_model_);
  gui->net_details_proxy_->setSourceModel(gui->main_network_model_);

  gui->ui.networkTreeView->setModel(gui->simple_net_proxy_);
  gui->ui.detailsTreeView->setModel(gui->net_details_proxy_);

  gui->ui.detailsTreeView->header()->resizeSection(0, 200);
  gui->ui.detailsTreeView->setItemDelegate(new PropertyEditorsDelegate());
  gui->ui.detailsTreeView->setSortingEnabled(true);
  gui->ui.detailsTreeView->sortByColumn(0, Qt::SortOrder::AscendingOrder);

  gui->setWindowTitle(tr("RDMnet Controller GUI"));

  networkTreeHeaderView = gui->ui.networkTreeView->header();
  networkTreeHeaderView->hideSection(1);
  networkTreeHeaderView->setSectionResizeMode(0, QHeaderView::Fixed);

  connect(gui->ui.networkTreeView->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), gui,
          SLOT(networkTreeViewSelectionChanged(const QItemSelection &, const QItemSelection &)));
  // connect(gui->ui.detailsTreeView, SIGNAL(activated(const QModelIndex &)),
  // gui, SLOT(detailsTreeViewActivated(const QModelIndex &)));

  connect(gui->ui.addBrokerByScopeButton, SIGNAL(clicked()), gui, SLOT(addScopeTriggered()));
  connect(gui->ui.newScopeNameEdit, SIGNAL(returnPressed()), gui, SLOT(addScopeTriggered()));
  connect(gui, SIGNAL(addScopeActivated(std::string)), gui->main_network_model_, SLOT(addScopeToMonitor(std::string)));

  connect(gui->ui.removeSelectedBrokerButton, SIGNAL(clicked()), gui, SLOT(removeSelectedBrokerTriggered()));
  connect(gui, SIGNAL(removeSelectedBrokerActivated(BrokerItem *)), gui->main_network_model_,
          SLOT(removeBroker(BrokerItem *)));

  connect(gui->ui.removeAllBrokersButton, SIGNAL(clicked()), gui, SLOT(removeAllBrokersTriggered()));
  connect(gui, SIGNAL(removeAllBrokersActivated()), gui->main_network_model_, SLOT(removeAllBrokers()));

  connect(gui->ui.resetDeviceButton, SIGNAL(clicked()), gui, SLOT(resetDeviceTriggered()));
  connect(gui, SIGNAL(resetDeviceActivated(ResponderItem *)), gui->main_network_model_,
          SLOT(resetDevice(ResponderItem *)));

  connect(gui->ui.networkTreeView, SIGNAL(expanded(const QModelIndex &)), gui->simple_net_proxy_,
          SLOT(directChildrenRevealed(const QModelIndex &)));
  connect(gui->simple_net_proxy_, SIGNAL(expanded(const QModelIndex &)), gui->main_network_model_,
          SLOT(directChildrenRevealed(const QModelIndex &)));

  connect(gui->ui.moreBrokerSettingsButton, SIGNAL(clicked()), gui, SLOT(openBrokerStaticAddDialog()));

  connect(gui, SIGNAL(addBrokerByIPActivated(std::string, const LwpaSockaddr &)), gui->main_network_model_,
          SLOT(addBrokerByIP(std::string, const LwpaSockaddr &)));

  connect(gui->main_network_model_, SIGNAL(brokerItemTextUpdated(const BrokerItem *)), gui,
          SLOT(processBrokerItemTextUpdate(const BrokerItem *)));

  connect(gui->main_network_model_, SIGNAL(resetDeviceSupportChanged(const class RDMnetNetworkItem *)), gui,
          SLOT(processResetDeviceSupportChanged(const class RDMnetNetworkItem *)));

  connect(gui->main_network_model_, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), gui,
          SLOT(dataChangeTest1(QModelIndex, QModelIndex, QVector<int>)));
  connect(gui->net_details_proxy_, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), gui,
          SLOT(dataChangeTest2(QModelIndex, QModelIndex, QVector<int>)));

  return gui;
}

void RDMnetControllerGUI::networkTreeViewSelectionChanged(const QItemSelection &selected,
                                                          const QItemSelection & /*deselected*/)
{
  if (!selected.indexes().isEmpty())
  {
    QModelIndex selectedIndex = selected.indexes().first();

    if (selectedIndex.isValid())
    {
      QModelIndex sourceIndex, proxyIndex;
      QStandardItem *selectedItem = NULL;
      RDMnetNetworkItem *netItem = NULL;

      sourceIndex = simple_net_proxy_->mapToSource(selectedIndex);
      selectedItem = main_network_model_->itemFromIndex(sourceIndex);
      ui.detailsTreeView->clearSelection();
      net_details_proxy_->setCurrentParentItem(selectedItem);

      proxyIndex = net_details_proxy_->mapFromSource(sourceIndex);
      net_details_proxy_->setCurrentParentIndex(proxyIndex);
      ui.detailsTreeView->setRootIndex(proxyIndex);

      netItem = dynamic_cast<RDMnetNetworkItem *>(selectedItem);

      if (netItem != NULL)
      {
        currently_selected_network_item_ = netItem;
        ui.resetDeviceButton->setEnabled(netItem->supportsResetDevice());
      }

      if (selectedItem->type() == BrokerItem::BrokerItemType)
      {
        currently_selected_broker_item_ = dynamic_cast<BrokerItem *>(selectedItem);
        ui.removeSelectedBrokerButton->setEnabled(true);
      }
      else
      {
        currently_selected_broker_item_ = NULL;
        ui.removeSelectedBrokerButton->setEnabled(false);
      }

      ui.currentSelectionLabel->setText(selectedItem->text());
    }
    else
    {
      currently_selected_broker_item_ = NULL;
      ui.removeSelectedBrokerButton->setEnabled(false);
      ui.currentSelectionLabel->setText(QString(""));
    }
  }
  else
  {
    currently_selected_broker_item_ = NULL;
    ui.removeSelectedBrokerButton->setEnabled(false);
    ui.currentSelectionLabel->setText(QString(""));
  }
}

void RDMnetControllerGUI::detailsTreeViewActivated(const QModelIndex &index)
{
  if (index.isValid())
  {
    QModelIndex sourceIndex = net_details_proxy_->mapToSource(index);
    QModelIndex proxyIndex = simple_net_proxy_->mapFromSource(sourceIndex);
    QStandardItem *item = main_network_model_->itemFromIndex(sourceIndex);

    if (item != NULL)
    {
      if (item->flags() & Qt::ItemIsSelectable)
      {
        ui.networkTreeView->expand(proxyIndex.parent());
        ui.networkTreeView->selectionModel()->select(proxyIndex, QItemSelectionModel::ClearAndSelect);

        net_details_proxy_->setCurrentParentIndex(index);
        ui.detailsTreeView->setRootIndex(index);
        ui.detailsTreeView->clearSelection();
      }
    }
  }
}

void RDMnetControllerGUI::addScopeTriggered()
{
  QString scopeText = ui.newScopeNameEdit->text();
  ui.newScopeNameEdit->clear();
  emit addScopeActivated(scopeText.toUtf8().constData());
}

void RDMnetControllerGUI::removeSelectedBrokerTriggered()
{
  if (currently_selected_broker_item_ != NULL)
  {
    if (net_details_proxy_->currentParentIsChildOfOrEqualTo(currently_selected_broker_item_))
    {
      ui.detailsTreeView->clearSelection();
      ui.detailsTreeView->setRootIndex(QModelIndex());

      net_details_proxy_->setFilterEnabled(false);
      net_details_proxy_->clearCurrentParentIndex();
      net_details_proxy_->setCurrentParentItem(NULL);
    }

    emit removeSelectedBrokerActivated(currently_selected_broker_item_);
    currently_selected_broker_item_ = NULL;

    ui.removeSelectedBrokerButton->setEnabled(false);
    ui.networkTreeView->clearSelection();

    net_details_proxy_->setFilterEnabled(true);
    net_details_proxy_->invalidate();

    ui.currentSelectionLabel->setText(QString(""));
  }
}

void RDMnetControllerGUI::removeAllBrokersTriggered()
{
  ui.networkTreeView->clearSelection();
  ui.detailsTreeView->clearSelection();
  ui.detailsTreeView->setRootIndex(QModelIndex());

  net_details_proxy_->setFilterEnabled(false);
  net_details_proxy_->clearCurrentParentIndex();
  net_details_proxy_->setCurrentParentItem(NULL);

  emit removeAllBrokersActivated();
  currently_selected_broker_item_ = NULL;

  net_details_proxy_->setFilterEnabled(true);
  net_details_proxy_->invalidate();

  ui.currentSelectionLabel->setText(QString(""));
}

void RDMnetControllerGUI::resetDeviceTriggered()
{
  if (currently_selected_network_item_ != NULL)
  {
    if (currently_selected_network_item_->type() == ResponderItem::ResponderItemType)
    {
      ResponderItem *device = dynamic_cast<ResponderItem *>(currently_selected_network_item_);

      if (device != NULL)
      {
        emit resetDeviceActivated(device);
      }
    }
  }
}

void RDMnetControllerGUI::openBrokerStaticAddDialog()
{
  BrokerStaticAddGUI *brokerStaticAddDialog = new BrokerStaticAddGUI(this, this);
  brokerStaticAddDialog->setAttribute(Qt::WA_DeleteOnClose);
  brokerStaticAddDialog->setWindowModality(Qt::WindowModal);
  brokerStaticAddDialog->setWindowTitle(tr("Add Broker by Static IP"));
  brokerStaticAddDialog->show();
}

void RDMnetControllerGUI::processBrokerItemTextUpdate(const BrokerItem *item)
{
  if (item != NULL)
  {
    if (item == currently_selected_broker_item_)
    {
      ui.currentSelectionLabel->setText(item->text());
    }
  }
}

void RDMnetControllerGUI::processResetDeviceSupportChanged(const RDMnetNetworkItem *item)
{
  if (currently_selected_network_item_ != NULL)
  {
    if (currently_selected_network_item_ == item)
    {
      ui.resetDeviceButton->setEnabled(item->supportsResetDevice() && item->isEnabled());
    }
  }
}

void RDMnetControllerGUI::dataChangeTest1(QModelIndex /*a*/, QModelIndex /*b*/, QVector<int> /*c*/)
{
}

void RDMnetControllerGUI::dataChangeTest2(QModelIndex /*a*/, QModelIndex /*b*/, QVector<int> /*c*/)
{
}

RDMnetControllerGUI::RDMnetControllerGUI(QWidget *parent)
    : QMainWindow(parent)
    , main_network_model_(NULL)
    , simple_net_proxy_(NULL)
    , net_details_proxy_(NULL)
    , currently_selected_broker_item_(NULL)
    , currently_selected_network_item_(NULL)
{
  ui.setupUi(this);
}