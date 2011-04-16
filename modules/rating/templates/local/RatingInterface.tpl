{*
 * $Revision: 17082 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{if $RatingSummary.firstCall}
{include file="gallery:modules/rating/templates/RatingImagePreload.tpl"}
{/if}

{if $RatingSummary.firstCall} {* suppress rating in album view *}
{if 0}
<p>Rating Data</p>
{foreach from=$RatingData key=key1 item=item1}
{g->text text="$key1: $item1<br>"}
{foreach from=$item1 key=key2 item=item2}
{g->text text="  $key2 => $item2<br>"}
{/foreach}
{/foreach}

<p>Rating Values</p>
{foreach from=$RatingSummary.ratingValues key=key item=item}
{g->text text="$key => $item"}
{/foreach}

<p>Rating names</p>
{foreach from=$RatingSummary.ratingNames key=key item=item}
{g->text text="$key => $item"}
{/foreach}
{/if}
<br>
{g->text text="&nbsp;&nbsp;Please rate each crew member below.  A single red spanner means good, and it gets worse from there on up.<br>&nbsp;&nbsp;Your ratings are shown in yellow."}
{foreach from=$RatingData key=dataKey item=dataItem}

<div class="giRatingUI">
    <table border="0" cellpadding="0" cellspacing="0"><tr><td width="8">
	</td><td>
	{foreach from=$RatingSummary.ratingValues item=ratingValue}{if $dataItem.canRate}<a
		href="javascript:rateItem({$dataItem.itemId}, {$dataItem.subItemId}, {$ratingValue}, '{g->url
			arg1="view=rating.RatingCallback" arg2="command=rate"
			arg3="itemId=`$dataItem.itemId`" arg4="subItemId=`$dataItem.subItemId`" arg5="rating=$ratingValue"
			arg6="authToken=__AUTHTOKEN__"}')"
		onmouseover="updateStarDisplay({$dataItem.itemId}, {$dataItem.subItemId}, {$ratingValue}); return true"
		onmouseout="resetStarDisplay({$dataItem.itemId}, {$dataItem.subItemId}); return true">{/if}<img
		src="{g->url href="modules/rating/images/transparent.gif"}"
		id="rating.star.{$dataItem.itemId}.{$dataItem.subItemId}.{$ratingValue}" class="giRatingUnit" alt=""
		title="{g->text text="Click a spanner to rate this item!"}"/>{if $dataItem.canRate}</a>{/if}{/foreach}

	<div class="giRatingAverageContainer">
		<div class="giRatingAverage" id="rating.averagePercent.{$dataItem.itemId}.{$dataItem.subItemId}"
			style="width:{$dataItem.averagePercent}%"></div></div>
	{if 0}
	<div class="giRatingVotes"><span
		id="rating.votes.{$dataItem.itemId}.{$dataItem.subItemId}">{g->text
		one="%s vote" many="%s votes" count=$dataItem.votes arg1=$dataItem.votes}</span>
	</div>
	{/if}
	<span class="giRatingHidden"
		id="rating.rating.{$dataItem.itemId}.{$dataItem.subItemId}">{$dataItem.rating}</span>
	<span class="giRatingHidden"
		id="rating.userRating.{$dataItem.itemId}.{$dataItem.subItemId}">{$dataItem.userRating}</span>
	</td><td width="8">
	</td><td width="100">
		{g->text text=$RatingSummary.ratingNames[$dataItem.subItemId]}
    </td><td>
	    <div class="giRatingVotes"><span
		id="rating.votes.{$dataItem.itemId}.{$dataItem.subItemId}">{g->text
		one="%s vote" many="%s votes" count=$dataItem.votes arg1=$dataItem.votes}</span>
	    </div>
	</td></tr></table>
</div>
<script type="text/javascript">
// <![CDATA[
resetStarDisplay({$dataItem.itemId}, {$dataItem.subItemId});
// ]]>
</script>

{/foreach}
{/if}
