<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

GalleryCoreApi::requireOnce('modules/notification/classes/NotificationHelper.class');

/**
 * This controller handles event / method preferences for the user on a per-item basis.
 * @package Notification
 * @subpackage UserInterface
 * @author Zimzat <zimzat@zimzat.com>
 * @version $Revision: 847 $
 */
class NotificationItemController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	$redirect = $status = $error = array();

	/* Check that the user is logged in */
	list ($ret, $userId) = NotificationHelper::getUserIdCheck();
	if ($ret) {
	    return array($ret, null);
	}

	$itemId = (int)GalleryUtilities::getRequestVariables('itemId');
	list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	if ($ret) {
	    return array($ret, null);
	}

	$redirect = array(
	    'view' => 'notification.NotificationItem', 
	    'itemId' => $itemId);

	if (!empty($form['action']['save'])) {
	    /* Load the Notification System */
	    list ($ret, $notificationSystem) = GalleryCoreApi::newFactoryInstance(
		'NotificationSystemInterface_1_0');
	    if ($ret) {
		return array($ret, null);
	    } else if (!isset($notificationSystem)) {
		return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
	    }

	    /* Get Enabled Methods */
	    list ($ret, $methods) = NotificationHelper::getMethods(NOTIFICATION_RETURN_BASIC);
	    if ($ret) {
		return array($ret, null);
	    }

	    /* Get Enabled Events */
	    list ($ret, $events) = NotificationHelper::getEvents(NOTIFICATION_RETURN_BASIC);
	    if ($ret) {
		return array($ret, null);
	    }
	    foreach ($events as $eventClass) {
		unset($eventInstance);
		list ($ret, $eventInstance) = GalleryCoreApi::newFactoryInstance(
		    'NotificationEventInterface_1_0', $eventClass);
		if ($ret) {
		    return array($ret, null);
		} else if (!isset($eventInstance)) {
		    return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		list ($ret, $enabled) = $eventInstance->getItemWatchPermission($item);
		if ($ret) {
		    return array($ret, null);
		} else if (!$enabled) {
		    unset($events[$eventClass]);
		    continue;
		}
	    }

	    /* Check events against form input for changes */
	    $changes = false;
	    foreach ($events as $eventClass) {
		/* Check for basic valid form input */
		if (empty($form['events'][$eventClass]) 
			|| !is_array($form['events'][$eventClass])) {
		    $error[] = 'form[error][event][invalid]';
		    break;
		}

		/* Get the subscription status to the event for this item */
		list ($ret, $methodClass) = $notificationSystem->isSubscribed(
		    $eventClass, $userId, $item);
		if ($ret) {
		    return array($ret, null);
		}

		/* Check to see if the event was in the form submitted */
		if (!isset($form['events'][$eventClass]['subscribe'])) {
		    if ($methodClass) {
			$unsubscribeFilter[] = 'i=' . $itemId;
		    }

		    continue;
		}

		/* Check to see if a methodClass was in the form and that it is valid. */
		if (!isset($form['events'][$eventClass]['methodClass']) 
			|| !in_array($form['events'][$eventClass]['methodClass'], $methods)) {
		    $error[] = 'form[error][event][method][invalid]';

		    continue; /* break; ? */
		}

		/* Subscribe to event */
		if ($form['events'][$eventClass]['methodClass'] !== $methodClass) {
		    list ($ret, $subscribed) = $notificationSystem->subscribeEvent(
			$eventClass, $userId, 'i=' . $itemId, 
			$form['events'][$eventClass]['methodClass']);
		    if ($ret) {
			return array($ret, null);
		    }

		    $changes = true;
		    continue;
		}
	    }

	    /* Unsubscribe the event; no check to see if it already existed needed */
	    if (!empty($unsubscribeFilter)) {
		list ($ret, $unsubscribed) = $notificationSystem->unsubscribeEvent(
		    $eventClass, $userId, $unsubscribeFilter);
		if ($ret) {
		    return array($ret, null);
		}
		$changes = true;
	    }

	    /* Alert of the changes we have done. */
	    $status['saved'] = 1;
	}

	$results['redirect'] = $redirect;
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * This view shows all event / method preferences for the user on a per-item basis.
 */
class NotificationItemView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	/* Check that the user is logged in */
	list ($ret, $userId) = NotificationHelper::getUserIdCheck();
	if ($ret) {
	    return array($ret, null);
	}

	$methodOptions = $eventInstances = array();
	$itemId = (int)GalleryUtilities::getRequestVariables('itemId');
	list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	if ($ret) {
	    return array($ret, null);
	}

	/* Methods */
	list ($ret, $methods) = NotificationHelper::getMethods();
	if ($ret) {
	    return array($ret, null);
	}
	foreach ($methods as $methodClass => $method) {
	    $methodOptions[$methodClass] = $method['methodName'];
	}

	/* Events */
	list ($ret, $events) = NotificationHelper::getEvents();
	if ($ret) {
	    return array($ret, null);
	}
	foreach ($events as $eventClass => $event) {
	    list ($ret, $enabled) = $event['eventInstance']->getItemWatchPermission($item);
	    if ($ret) {
		return array($ret, null);
	    } else if (!$enabled) {
		unset($events[$eventClass]);
		continue;
	    }
	}

	/* Subscriptions */
	list ($ret, $notificationSystem) = GalleryCoreApi::newFactoryInstance(
	    'NotificationSystemInterface_1_0');
	if ($ret) {
	    return array($ret, null);
	} else if (!isset($notificationSystem)) {
	    return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
	}
	list ($ret, $subscriptions) = $notificationSystem->getSubscriptions(
	    array_keys($events), array('userId' => $userId, 'itemId' => $itemId));
	if ($ret) {
	    return array($ret, null);
	}

	/* Go through the list of subscriptions and only pass on those directly for this item */
	foreach ($subscriptions as $eventClass => $eventSubscriptions) {
	    unset($subscriptions[$eventClass]);
	    foreach ($eventSubscriptions as $eventSubscription) {
		if ($eventSubscription['filter'] == 'i=' . $itemId) {
		    $subscriptions[$eventClass] = $eventSubscription['methodClass'];
		    break;
		}
	    }
	}

	$template->setVariable('NotificationItem', array('itemId' => $itemId));
	$template->setVariable('NotificationMethods', $methods);
	$template->setVariable('NotificationMethodOptions', $methodOptions);
	$template->setVariable('NotificationEvents', $events);
	$template->setVariable('NotificationSubscriptions', $subscriptions);

	return array(null, array('body' => 'modules/notification/templates/NotificationItem.tpl'));
    }
}
?>
