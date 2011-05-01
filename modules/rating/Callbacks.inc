<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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

/**
 * @package Rating
 * @author  Don Seiler <don@seiler.us>
 * @version $Revision: 17580 $
 */
class RatingCallbacks {

    function callback($params, &$smarty, $callback, $userId) {
	global $gallery;

	switch ($callback) {
	case 'LoadRating':
	    $itemId = $params['itemId'];
	    list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId, 'GalleryItem');
	    if ($ret) {
		return $ret;
	    }

	    list ($ret, $hasPermission) =
		GalleryCoreApi::hasItemPermission($item->getId(), 'core.view');
	    if ($ret) {
		return $ret;
	    }
	    if (!$hasPermission) {
		/* Avoid information disclosure, act as if the item didn't exist. */
		return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
	    }

	    if (GalleryUtilities::isA($item, 'GalleryAlbumItem')) {
		$albumId = $itemId;
		list ($ret, $allowAlbumRating) =
		    GalleryCoreApi::getPluginParameter('module', 'rating', 'allowAlbumRating');
		if ($ret) {
		    return $ret;
		}
		if (!$allowAlbumRating) {
		    return null;
		}
	    } else {
		$albumId = $item->getParentId();
	    }

	    list ($ret, $enabled) =
		GalleryCoreApi::getPluginParameter('module', 'rating', 'enabled', $albumId);
	    if ($ret) {
		return $ret;
	    }
	    if (!$enabled) {
		return null;
	    }

	    list ($ret, $permission) =
		GalleryCoreApi::hasItemPermission($itemId, 'rating.view', $userId);
	    if ($ret) {
		return $ret;
	    }
	    if (!$permission) {
		return null;
	    }

	    GalleryCoreApi::requireOnce('modules/rating/classes/RatingHelper.class');
	    list ($ret, $permission) = GalleryCoreApi::hasItemPermission($itemId, 'rating.add', $userId);
	    if ($ret) {
		return $ret;
	    }

        $itemTitle = $item->getTitle();
		$entryName = str_replace('First and', '', $itemTitle);
        
        if (preg_match('[ MW]1x', $entryName)) {
          $ratingSet = array('Sculler');
        }
        elseif (preg_match('[ MW](2x|2-)', $entryName)) {
          $ratingSet = array('Bow', 'Stroke');
        }
        elseif (preg_match('[ MW]2\+', $entryName)) {
          $ratingSet = array('Bow', 'Stroke', 'Cox');
        }
        elseif (preg_match('[ MW](4x\+|4X\+|4\+)', $entryName)) {
          $ratingSet = array('Bow', '2', '3', 'Stroke', 'Cox');
        }
        elseif (preg_match('[ MW](4x|4X|4-)', $entryName)) {
          $ratingSet = array('Bow', '2', '3', 'Stroke');
        }
        else {
          $ratingSet = array('Bow', '2', '3', '4', '5', '6', '7', 'Stroke', 'Cox');
        }
        
        $numBoats = substr_count($entryName, ' and ') + substr_count($entryName, ' then ') + 1;
		if ($numBoats <= 1) {
		    $ratingNames = $ratingSet;
        }
		elseif ($numBoats == 2) {
		    $ratingNames = array('<b>Leading/Nearest Boat:</b> Bow', '2', '3', '4', '5', '6', '7', 'Stroke', 'Cox', 
							'<b>Following/Farthest Boat:</b> Bow', '2', '3', '4', '5', '6', '7', 'Stroke', 'Cox');
        }
        else
		{
			$ratingNames = array('<b>Leading/Nearest Boat:</b> Bow', '2', '3', '4', '5', '6', '7', 'Stroke', 'Cox',
					'<b>Middle Boat:</b><br>Bow', '2', '3', '4', '5', '6', '7', 'Stroke', 'Cox',
					'<b>Following/Farthest Boat:</b> Bow', '2', '3', '4', '5', '6', '7', 'Stroke', 'Cox');
		}
        if ($itemId == '2382') {
            $ratingNames[] = 'Whoever rigged the boat';
       }
       if (substr_count($entryName, '-flag-') == 1) {
            $ratingNames[] = 'Flag provider';
       }

		$RatingDataElem = array();
		for ($i = 0; $i < count($ratingNames); $i++) {
		    list ($ret, $RatingDataElem) = RatingHelper::fetchRatings(array($itemId), $userId, $i);
    	    if ($ret) {
	    	    return $ret;
            }
			$RatingDataElem[$itemId]['canRate'] = ($permission ? true : false);
    		$RatingData[$itemId][$i] = $RatingDataElem[$itemId];
		}



	    $session =& $gallery->getSession();
	    $authToken = $session->isPersistent() ? $session->getAuthToken() : '';
	    $RatingSummary = array('ratingValues' => array(1, 2, 3, 4, 5),
				   'firstCall' => true, 'authToken' => $authToken,
				   'ratingNames' => $ratingNames);

	    $block =& $smarty->_tpl_vars['block'];
	    $block['rating']['RatingData'] = $RatingData[$itemId];
	    $block['rating']['RatingSummary'] = $RatingSummary;

	    return null;
	}

	return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
    }
}
?>
